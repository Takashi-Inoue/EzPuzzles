/*
 * Copyright 2021 Takashi Inoue
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

#ifndef THREADIMAGELOADER_H
#define THREADIMAGELOADER_H

#include <QThread>
#include <QPixmap>
#include <QReadWriteLock>
#include <QVariant>

class ThreadImageLoader : public QThread
{
    Q_OBJECT
public:
    using QThread::QThread;

    void addPath(QStringView filePathName, QVariant data = QVariant());
    void addPaths(const QStringList &filePathNames);
    void addItem(const QPair<QString, QVariant> &item);
    void addItems(const QList<QPair<QString, QVariant>> &items);
    void clear();

    void stop();

signals:
    void loaded(QString filePathName, QVariant data, QPixmap pixmap);
    void loadingFailed(QString filePathName, QVariant data);

protected:
    void run() override;

private:
    bool isStopRequested() const;

    mutable QReadWriteLock m_lock;

    bool m_isStopRequested = false;

    QList<QPair<QString, QVariant>> m_items;
};

#endif // THREADIMAGELOADER_H
