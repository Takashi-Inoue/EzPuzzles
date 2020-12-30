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

#include "ThreadImageLoader.h"

#include <QImage>

#include <QDebug>

void ThreadImageLoader::addPath(QStringView filePathName, QVariant data)
{
    QWriteLocker locker(&m_lock);

    m_items << qMakePair(filePathName.toString(), data);
}

void ThreadImageLoader::addPaths(const QStringList &filePathNames)
{
    QWriteLocker locker(&m_lock);

    for (QStringView path : filePathNames)
        m_items << qMakePair(path.toString(), QVariant());
}

void ThreadImageLoader::addItem(const QPair<QString, QVariant> &item)
{
    QWriteLocker locker(&m_lock);

    m_items << item;
}

void ThreadImageLoader::addItems(const QList<QPair<QString, QVariant>> &items)
{
    QWriteLocker locker(&m_lock);

    m_items += items;
}

void ThreadImageLoader::clear()
{
    QWriteLocker locker(&m_lock);

    m_items.clear();
}

void ThreadImageLoader::stop()
{
    QWriteLocker locker(&m_lock);

    qDebug() << QStringLiteral("ThreadImageLoader has received a stop request.");

    m_isStopRequested = true;
}

void ThreadImageLoader::run()
{
    QWriteLocker locker(&m_lock);

    m_isStopRequested = false;

    qInfo() << QStringLiteral("ThreadImageLoader has started loading images.");

    while (!m_items.isEmpty()) {
        QPair<QString, QVariant> item = m_items.takeFirst();

        locker.unlock();

        if (isStopRequested())
            return;

        QPixmap pixmap = QPixmap::fromImage(QImage(item.first));

        if (!pixmap.isNull()) {
            qInfo() << QStringLiteral("loading...OK. [%1]").arg(item.first);
            emit loaded(item.first, item.second, pixmap);
        } else {
            qInfo() << QStringLiteral("loading...failed. [%1]").arg(item.first);
            emit loadingFailed(item.first, item.second);
        }

        locker.relock();
    }

    qInfo() << QStringLiteral("ThreadImageLoader completed.");
}

bool ThreadImageLoader::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}
