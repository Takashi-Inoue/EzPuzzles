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

#include "ListWidgetImages.h"

#include <QApplication>
#include <QPainter>

ListWidgetImages::ListWidgetImages(QWidget *parent)
    : QListWidget(parent)
    , m_imageLoader(new ThreadImageLoader(this))
{
    setViewMode(QListView::IconMode);

    connect(m_imageLoader, &ThreadImageLoader::loaded, this, &ListWidgetImages::onImageLoaded);
    connect(m_imageLoader, &ThreadImageLoader::loadingFailed, this, &ListWidgetImages::onLoadingFailed);

    connect(this, &QAbstractItemView::iconSizeChanged, this, &ListWidgetImages::onIconSizeChanged);
}

ListWidgetImages::~ListWidgetImages()
{
    m_imageLoader->stop();
    m_imageLoader->wait();
}

void ListWidgetImages::addPath(QStringView imagePathName)
{
    if (imagePathName.isEmpty())
        return;

    int row = count();

    createNewItem(imagePathName);

    m_imageLoader->addPath(imagePathName, row);
    m_imageLoader->start();
}

void ListWidgetImages::addPaths(const QStringList &imagePathNames)
{
    if (imagePathNames.isEmpty())
        return;

    int row = count();

    QList<QPair<QString, QVariant>> items;

    for (QStringView path : imagePathNames)
        items << qMakePair(path.toString(), row++);

    createNewItem(imagePathNames);

    m_imageLoader->addItems(items);
    m_imageLoader->start();
}

QString ListWidgetImages::currentImagePathName() const
{
    QListWidgetItem *item = currentItem();

    if (item == nullptr)
        return QString();

    return item->data(PathNameRole).toString();
}

QStringList ListWidgetImages::allImagePathNames() const
{
    QStringList paths;
    int rowCount = count();

    paths.reserve(rowCount);

    for (int row = 0; row < rowCount; ++row)
        paths << item(row)->data(PathNameRole).toString();

    return paths;
}

void ListWidgetImages::onIconSizeChanged()
{
    int rowCount = count();

    for (int i = 0; i < rowCount; ++i)
        item(i)->setSizeHint(iconSize() + QSize(4, 4)); // do layout

    QList<QPair<QString, QVariant>> pathRowPairs;

    pathRowPairs.reserve(rowCount);

    for (int row = 0; row < rowCount; ++row)
        pathRowPairs << qMakePair(item(row)->data(PathNameRole).toString(), row);

    m_imageLoader->clear();
    m_imageLoader->wait();
    m_imageLoader->addItems(pathRowPairs);
    m_imageLoader->start();
}

void ListWidgetImages::onImageLoaded(QString /*imagePathName*/, QVariant data, QPixmap pixmap)
{
    const int edgeWidth = 5;

    QSize thumbAreaSize = iconSize() - QSize(edgeWidth * 2, edgeWidth * 2);
    QPixmap thumbnail(pixmap.scaled(thumbAreaSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPixmap iconPixmap(iconSize());
    iconPixmap.fill(Qt::transparent);

    QRect thumbnailRect = thumbnail.rect();
    thumbnailRect.moveCenter(iconPixmap.rect().center());

    QPainter painter(&iconPixmap);

    painter.setPen(QApplication::palette().midlight().color());
    painter.setBrush(QApplication::palette().light());
    painter.drawRect(thumbnailRect + QMargins(edgeWidth, edgeWidth, edgeWidth - 1, edgeWidth - 1));

    painter.drawPixmap(thumbnailRect, thumbnail);

    item(data.toInt())->setIcon(QIcon(iconPixmap));
}

void ListWidgetImages::onLoadingFailed(QString /*imagePathName*/, QVariant data)
{
    delete takeItem(data.toInt());
}

void ListWidgetImages::createNewItem(QStringView imagePathName)
{
    QPixmap pixmap(iconSize());

    pixmap.fill(QApplication::palette().base().color());

    auto item = new QListWidgetItem(QIcon(pixmap), QString(), this);

    item->setData(PathNameRole, imagePathName.toString());
}

void ListWidgetImages::createNewItem(const QStringList &imagePathNames)
{
    QPixmap pixmap(iconSize());

    pixmap.fill(QApplication::palette().base().color());

    for (QStringView imagePathName : imagePathNames) {
        auto item = new QListWidgetItem(QIcon(pixmap), QString(), this);

        item->setData(PathNameRole, imagePathName.toString());
    }
}
