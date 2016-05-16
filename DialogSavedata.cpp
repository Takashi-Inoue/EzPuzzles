/*
 * Copyright 2016 Takashi Inoue
 *
 * This file is part of EzPuzzles.
 *
 * EzPuzzles is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EzPuzzles is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EzPuzzles.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "DialogSavedata.h"
#include "ui_DialogSavedata.h"

#include "EzPuzzles.h"
#include "ThreadOperation.h"
#include "GameInfoLoader.h"
#include "ISaveData.h"

#include "fifteen/GameSimpleSlide.h"
#include "fifteen/GameSimpleSwap.h"
#include "mine/GameMineSweeper.h"

#include "MoveToTrashBox.h"

#include <QDateTime>
#include <QDir>
#include <QDataStream>
#include <QFileInfo>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QDebug>

class SaveInfoDelegate : public QStyledItemDelegate
{
public:
    SaveInfoDelegate() = default;
    ~SaveInfoDelegate() = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        option.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

        QRect drawRect(option.rect - QMargins(2, 0, 0, 0));

        QIcon ico = index.data(Qt::DecorationRole).value<QIcon>();

        if (!ico.isNull()) {
            QIcon::Mode mode = QIcon::Normal;

            if (!(option.state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if (option.state & QStyle::State_Selected)
                mode = QIcon::Selected;
            else if (option.state & QStyle::State_Active)
                mode = QIcon::Active;

            ico.paint(painter, drawRect, Qt::AlignLeft | Qt::AlignVCenter, mode);

            drawRect.setLeft(drawRect.left() + ico.actualSize(drawRect.size()).width() + 2);
        }

        auto gameInfo = reinterpret_cast<ISaveData *>(index.data(Qt::UserRole).toULongLong());

        if (gameInfo == nullptr)
            return;

        painter->save();

        painter->setRenderHint(QPainter::TextAntialiasing);

        drawRect -= QMargins(0, 0, 1, 1);

        QFont defaultFont = option.widget->font();
        QFont gameNameFont = defaultFont;

        gameNameFont.setItalic(true);
        gameNameFont.setBold(true);
        gameNameFont.setPointSizeF(gameNameFont.pointSizeF() * 1.1);

        painter->setFont(gameNameFont);

        painter->drawText(drawRect, Qt::AlignLeft | Qt::AlignTop, gameInfo->gameName());

        painter->setFont(defaultFont);
        painter->drawText(drawRect, Qt::AlignRight | Qt::AlignBottom, index.data().toString() + "\n" + QFileInfo(gameInfo->imageFilePath()).baseName());

        painter->restore();
    }
};

DialogSavedata::DialogSavedata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSavedata),
    infoLoader(nullptr)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    gameTypeMap[Fifteen::GameSimpleSlide::gameName()] = OnlySimpleSlide;
    gameTypeMap[Fifteen::GameSimpleSwap::gameName()] = OnlySwap;
    gameTypeMap[MineSweeper::GameMineSweeper::gameName()] = OnlyMineSweeper;

    initComboBox();
    initListWidget();

    infoLoader = new GameInfoLoader(savedataNames);
    infoLoader->moveToThread(&loadThread);

    connect(infoLoader, SIGNAL(loaded(QString,ISaveData*)), this, SLOT(saveInfoLoaded(QString,ISaveData*)));

    loadThread.start();
    infoLoader->start();
}

DialogSavedata::~DialogSavedata()
{
    infoLoader->stop();
    infoLoader->wait();

    loadThread.quit();
    loadThread.wait();

    delete ui;
}

IGame *DialogSavedata::loadGame() const
{
    auto selectedItems = ui->listWidget->selectedItems();

    if (selectedItems.isEmpty())
        return nullptr;

    auto savedata = getSaveDataFromListItem(selectedItems.first());

    if (savedata == nullptr)
        return nullptr;

    return savedata->loadGame();
}

void DialogSavedata::saveInfoLoaded(QString savedataName, ISaveData *gameInfo)
{
    int index = savedataNames.indexOf(savedataName);

    if (index == -1)
        return;

    auto item = ui->listWidget->item(index);

    QString lastModified = QFileInfo(EzPuzzles::saveDirPath() + "/" + savedataName).lastModified().toString(Qt::SystemLocaleLongDate);

    item->setText(lastModified);
    item->setIcon(gameInfo->gameTypeIcon());
    item->setData(Qt::UserRole, reinterpret_cast<uintptr_t>(gameInfo));
    item->setData(Qt::UserRole + 1, gameTypeMap.value(gameInfo->gameName()));
}

void DialogSavedata::on_listWidget_itemSelectionChanged()
{
    ui->labelInformations->setVisible(true);
    ui->imageWidget->setVisible(true);

    auto selectionModel = ui->listWidget->selectionModel();

    selectionModel->hasSelection() ? onDataSelected(selectionModel->selectedRows().first().row()) // single select
                                   : onDataSelectionCleared();
}

void DialogSavedata::on_comboBox_currentIndexChanged(int index)
{
    ShownDataType shownDataType = static_cast<ShownDataType>(ui->comboBox->itemData(index).toInt());

    shownDataType == ShowAll ? showAllSaveData()
                             : showSelectedTypeData(shownDataType);

    auto items = ui->listWidget->selectedItems();

    if (!items.isEmpty()) {
        bool isVisible = !items.first()->isHidden(); // single select

        ui->pushButtonRemove->setEnabled(isVisible);
        ui->imageWidget->setVisible(isVisible);
        ui->labelInformations->setVisible(isVisible);
    }
}

void DialogSavedata::on_pushButtonRemove_clicked()
{
    auto selectedIndexes = ui->listWidget->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
        return;

    int row = selectedIndexes.first().row();
    QString savedataName = savedataNames.at(row);

    QString savedataPath = EzPuzzles::saveDirPath() + "/" + savedataName;
    QString ssPath = savedataPath.left(savedataPath.size() - 3) + "png";

    MoveToTrashBox(savedataPath).exec();
    MoveToTrashBox(ssPath).exec();

    ui->listWidget->model()->removeRow(row);
    savedataNames.removeAt(row);
}

ISaveData *DialogSavedata::getSaveDataFromListItem(const QListWidgetItem *item) const
{
    return reinterpret_cast<ISaveData *>(item->data(Qt::UserRole).toULongLong());
}

void DialogSavedata::onDataSelectionCleared()
{
    ui->pushButtonRemove->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->labelInformations->clear();
    ui->imageWidget->setPixmap(QPixmap());
}

void DialogSavedata::onDataSelected(int row)
{
    ui->pushButtonRemove->setEnabled(true);

    QString dataname = savedataNames.at(row);
    QString ssPath = EzPuzzles::saveDirPath() + "/" + dataname.left(dataname.size() - 3) + "png";

    ui->imageWidget->setPixmap(QPixmap::fromImage(QImage(ssPath)));
    ui->imageWidget->repaint();

    auto savedata = getSaveDataFromListItem(ui->listWidget->item(row));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(savedata != nullptr && savedata->isValid()); // savedata == null のとき false を与えるように

    if (savedata != nullptr)
        ui->labelInformations->setText(savedata->informations().join('\n'));
}

void DialogSavedata::showAllSaveData()
{
    for (int i = 0, lim = ui->listWidget->model()->rowCount(); i < lim; ++i)
        ui->listWidget->item(i)->setHidden(false);
}

void DialogSavedata::showSelectedTypeData(ShownDataType shownDataType)
{
    Q_ASSERT(shownDataType != ShowAll);

    for (int i = 0, lim = ui->listWidget->model()->rowCount(); i < lim; ++i) {
        auto item = ui->listWidget->item(i);
        auto type = static_cast<ShownDataType>(item->data(Qt::UserRole + 1).toInt());

        item->setHidden(type != shownDataType);
    }
}

void DialogSavedata::initComboBox()
{
    auto view = new QListView();

    view->setUniformItemSizes(true);
    view->setAlternatingRowColors(true);

    ui->comboBox->setView(view);
    ui->comboBox->setStyleSheet("QAbstractItemView::item {min-height: 20px;}");

    ui->comboBox->addItem(QIcon(":/ico/gameAll"),         "All",         ShowAll);
    ui->comboBox->addItem(QIcon(":/ico/gameSimpleSlide"), "Slide",       OnlySimpleSlide);
    ui->comboBox->addItem(QIcon(":/ico/gameSwap"),        "Swap",        OnlySwap);
    ui->comboBox->addItem(QIcon(":/ico/gameMine"),        "MineSweeper", OnlyMineSweeper);
}

void DialogSavedata::initListWidget()
{
    QDir saveDir(EzPuzzles::saveDirPath());

    savedataNames = saveDir.entryList({"*.dat"}, QDir::Files, QDir::Time);

    ui->listWidget->setItemDelegate(new SaveInfoDelegate());
    ui->listWidget->setWordWrap(true);
    ui->listWidget->setStyleSheet("QListWidget::item {min-height: 40px;}");

    QPixmap pixmap(ui->listWidget->iconSize());
    pixmap.fill(Qt::white);

    QIcon icon(pixmap);

    for (auto &name : savedataNames)
        ui->listWidget->addItem(new QListWidgetItem(icon, name));
}
