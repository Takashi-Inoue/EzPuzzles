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
#include "IGame.h"
#include "SourceImage.h"
#include "ThreadGameInfoLoader.h"

#include <QDateTime>
#include <QDir>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QDebug>

class SaveInfoDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        option.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

        QRect drawRect(option.rect - QMargins(2, 0, 0, 0));

        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();

        if (!icon.isNull()) {
            QIcon::Mode mode = QIcon::Normal;

            if (!(option.state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if (option.state & QStyle::State_Selected)
                mode = QIcon::Selected;
            else if (option.state & QStyle::State_Active)
                mode = QIcon::Active;

            icon.paint(painter, drawRect, Qt::AlignLeft | Qt::AlignVCenter, mode);

            drawRect.setLeft(drawRect.left() + icon.actualSize(drawRect.size()).width() + 2);
        }

        auto savedata = index.data(Qt::UserRole).value<QSharedPointer<AbstractSaveData>>();

        if (savedata == nullptr)
            return;

        painter->save();

        painter->setRenderHint(QPainter::TextAntialiasing);

        drawRect -= QMargins(0, 0, 1, 1);

        QFont defaultFont = option.widget->font();
        QFont gameNameFont = defaultFont;

        gameNameFont.setPointSizeF(defaultFont.pointSizeF() * 1.1);
        gameNameFont.setItalic(true);
        gameNameFont.setBold(true);

        painter->setFont(gameNameFont);

        painter->drawText(drawRect, Qt::AlignLeft | Qt::AlignTop, savedata->gameTypeName());

        painter->setFont(defaultFont);

        QString subInfo = QStringLiteral("%1\n%2")
                         .arg(index.data().toString(), savedata->sourceImage().baseName());

        painter->drawText(drawRect, Qt::AlignRight | Qt::AlignBottom, subInfo);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const override
    {
        QFontInfo fontInfo(option.widget->font());

        return QSize(fontInfo.pixelSize() * 18, fontInfo.pixelSize() * 4);
    }
};

DialogSavedata::DialogSavedata(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSavedata)
    , m_threadInfoLoader(new ThreadGameInfoLoader(this))
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    initComboBox();
    initListWidget();

    int pixelSize = ui->labelInformations->fontInfo().pixelSize();
    ui->frameLabel->setMinimumWidth(pixelSize * 20);

    m_threadInfoLoader->setSaveDataNames(m_savedataNames);
    connect(m_threadInfoLoader, &ThreadGameInfoLoader::loaded, this, &DialogSavedata::onSaveInfoLoaded);
    m_threadInfoLoader->start();
}

DialogSavedata::~DialogSavedata()
{
    m_threadInfoLoader->stop();
    m_threadInfoLoader->wait();

    delete ui;
}

QSharedPointer<IGame> DialogSavedata::loadGame() const
{
    QListWidgetItem *item = ui->listWidget->currentItem();

    if (item == nullptr)
        return nullptr;

    auto savedata = saveDataFromListItem(item);

    if (savedata == nullptr)
        return nullptr;

    return savedata->loadGame();
}

void DialogSavedata::onSaveInfoLoaded(QString savedataName, QSharedPointer<AbstractSaveData> gameInfo)
{
    int index = int(m_savedataNames.indexOf(savedataName));

    if (index == -1)
        return;

    QString lastModified = QFileInfo(saveDataPathName(index)).lastModified().toString(Qt::ISODate);
    QListWidgetItem *item = ui->listWidget->item(index);

    item->setText(lastModified);
    item->setIcon(gameInfo->gameTypeIcon());
    item->setData(Qt::UserRole, QVariant::fromValue<QSharedPointer<AbstractSaveData>>(gameInfo));
}

void DialogSavedata::setUIState()
{
    bool isEnableAndVisible = false;

    QListWidgetItem *item = ui->listWidget->currentItem();

    if (item)
        isEnableAndVisible = !item->isHidden();

    ui->pushButtonRemove->setEnabled(isEnableAndVisible);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isEnableAndVisible);
    ui->labelInformations->setVisible(isEnableAndVisible);
    ui->imageWidget->setVisible(isEnableAndVisible);
}

void DialogSavedata::onDataSelected(int row)
{
    if (row == -1)
        return;

    ui->imageWidget->setPixmap(QPixmap::fromImage(QImage(thumbnailPathName(row))));
    ui->imageWidget->repaint();

    QSharedPointer<AbstractSaveData> savedata = saveDataFromListItem(ui->listWidget->item(row));

    if (savedata != nullptr)
        ui->labelInformations->setText(savedata->informations().join('\n'));
}

void DialogSavedata::on_comboBox_currentIndexChanged(int index)
{
    auto shownDataType = ShownData(ui->comboBox->itemData(index).toInt());

    shownDataType == ShownData::all ? showAllSaveData()
                                    : showSpecifiedTypeData(shownDataType);
    setUIState();
}

void DialogSavedata::on_pushButtonRemove_clicked()
{
    int row = ui->listWidget->currentRow();

    if (row == -1)
        return;

    if (!QFile(saveDataPathName(row)).moveToTrash()) {
        qInfo() << QStringLiteral("Failed to move to trash can. [%1]").arg(saveDataPathName(row));
        return;
    }

    if (!QFile(thumbnailPathName(row)).moveToTrash())
        qInfo() << QStringLiteral("Failed to move to trash can. [%1]").arg(thumbnailPathName(row));

    ui->listWidget->takeItem(row);
    m_savedataNames.removeAt(row);
}

QString DialogSavedata::saveDataPathName(int row) const
{
    return QStringLiteral("%1/%2").arg(EzPuzzles::saveDirPath(), m_savedataNames[row]);
}

QString DialogSavedata::thumbnailPathName(int row) const
{
    return QStringLiteral("%1/%2png").arg(EzPuzzles::saveDirPath(), m_savedataNames[row].chopped(3));
}

QSharedPointer<AbstractSaveData> DialogSavedata::saveDataFromListItem(const QListWidgetItem *item) const
{
    return item->data(Qt::UserRole).value<QSharedPointer<AbstractSaveData>>();
}

void DialogSavedata::showAllSaveData()
{
    for (int i = 0, count = ui->listWidget->count(); i < count; ++i)
        ui->listWidget->item(i)->setHidden(false);
}

void DialogSavedata::showSpecifiedTypeData(ShownData shownDataType)
{
    if (shownDataType == ShownData::all) {
        showAllSaveData();
        return;
    }

    static const QMap<ShownData, QList<EzPuzzles::GameType>> showTypeMap = {
        {ShownData::typeSlide,   {EzPuzzles::SimpleSlide}},
        {ShownData::typeSwap,    {EzPuzzles::SimpleSwap}},
        {ShownData::simpleSlide, {EzPuzzles::SimpleSlide}},
        {ShownData::simpleSwap,  {EzPuzzles::SimpleSwap}},
        {ShownData::mineSweeper, {EzPuzzles::MineSweeper}},
    };

    const QList<EzPuzzles::GameType> &types = showTypeMap[shownDataType];

    for (int i = 0, count = ui->listWidget->count(); i < count; ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);

        item->setHidden(!types.contains(saveDataFromListItem(item)->gameType()));
    }
}

void DialogSavedata::initComboBox()
{
    auto view = new QListView();

    view->setUniformItemSizes(true);
    view->setAlternatingRowColors(true);

    ui->comboBox->setView(view);

    ui->comboBox->addItem(QIcon(QStringLiteral(":/icons/all")),   QStringLiteral("All"),         int(ShownData::all));
    ui->comboBox->addItem(QIcon(QStringLiteral(":/icons/slide")), QStringLiteral("Slide"),       int(ShownData::simpleSlide));
    ui->comboBox->addItem(QIcon(QStringLiteral(":/icons/swap")),  QStringLiteral("Swap"),        int(ShownData::simpleSwap));
    ui->comboBox->addItem(QIcon(QStringLiteral(":/icons/mine")),  QStringLiteral("MineSweeper"), int(ShownData::mineSweeper));
}

void DialogSavedata::initListWidget()
{
    QDir saveDir(EzPuzzles::saveDirPath());

    m_savedataNames = saveDir.entryList({"*.dat"}, QDir::Files, QDir::Time);

    ui->listWidget->setItemDelegate(new SaveInfoDelegate(ui->listWidget));
    ui->listWidget->setWordWrap(true);

    QPixmap pixmap(ui->listWidget->iconSize());
    pixmap.fill(Qt::white);

    QIcon icon(pixmap);

    for (auto &name : m_savedataNames)
        ui->listWidget->addItem(new QListWidgetItem(icon, name));

    int fontPixelSize = ui->listWidget->fontInfo().pixelSize();

    ui->listWidget->setMinimumWidth(fontPixelSize * 21);
    ui->listWidget->setMinimumHeight(fontPixelSize * 25);
}
