/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "DialogSavedata.h"
#include "ui_DialogSavedata.h"

#include "EzPuzzles.h"
#include "SaveManager.h"
#include "ThreadOperation.h"
#include "SaveInfoLoader.h"

#include "GameFifteen.h"
#include "GameSwap.h"
#include "mine/GameMineSweeper.h"

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

        QStringList strings = index.data().toString().split("|");

        if (strings.size() < 3)
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

        painter->drawText(drawRect, Qt::AlignLeft | Qt::AlignTop, strings.front());

        painter->setFont(defaultFont);
        painter->drawText(drawRect, Qt::AlignRight | Qt::AlignBottom, strings.at(1) + "\n" + strings.back());

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

    initComboBox();
    initListWidget();

    infoLoader = new SaveInfoLoader(savedataNames);
    infoLoader->moveToThread(&loadThread);

    connect(infoLoader, SIGNAL(loaded(QString,QString,QString)), this, SLOT(saveInfoLoaded(QString,QString,QString)));

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

void DialogSavedata::saveInfoLoaded(QString savedataName, QString gameName, QString imageBaseName)
{
    int index = savedataNames.indexOf(savedataName);

    if (index == -1)
        return;

    auto item = ui->listWidget->item(index);

    QString created = QFileInfo(EzPuzzles::saveDirPath() + "/" + savedataName).created().toString(Qt::SystemLocaleLongDate);

    item->setText(gameName + "|" + created + "|" + imageBaseName);

    if (gameName == GameFifteen::gameName())
        item->setIcon(QIcon(":/ico/gameSimpleSlide"));

    if (gameName == GameSwap::gameName())
        item->setIcon(QIcon(":/ico/gameSwap"));

    if (gameName == MineSweeper::GameMineSweeper::gameName())
        item->setIcon(QIcon(":/ico/gameMine"));
}

void DialogSavedata::on_listWidget_itemSelectionChanged()
{
    bool hasSelection = ui->listWidget->selectionModel()->hasSelection();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasSelection);

    if (!hasSelection) {
        ui->imageWidget->setPixmap(QPixmap());

        return;
    }

    int row = ui->listWidget->selectionModel()->selectedRows().first().row();

    QString ssPath = EzPuzzles::saveDirPath() + "/" + QFileInfo(savedataNames.at(row)).baseName() + ".png";

    ui->imageWidget->setPixmap(QPixmap::fromImage(QImage(ssPath)));
    ui->imageWidget->repaint();
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

    savedataNames = saveDir.entryList(SaveManager::filterExtensions(), QDir::Files, QDir::Time);

    ui->listWidget->setItemDelegate(new SaveInfoDelegate());
    ui->listWidget->setWordWrap(true);
    ui->listWidget->setStyleSheet("QListWidget::item {min-height: 40px;}");

    QPixmap pixmap(ui->listWidget->iconSize());
    pixmap.fill(Qt::white);

    QIcon icon(pixmap);

    for (auto &name : savedataNames)
        ui->listWidget->addItem(new QListWidgetItem(icon, name));
}
