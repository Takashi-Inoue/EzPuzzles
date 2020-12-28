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

#ifndef THREADGAMEINFOLOADER_H
#define THREADGAMEINFOLOADER_H

#include <QThread>
#include <QReadWriteLock>
#include <QSharedPointer>

#include "AbstractSaveData.h"

class ThreadGameInfoLoader : public QThread
{
    Q_OBJECT
public:
    using QThread::QThread;

    void setSaveDataNames(QStringList saveDataNames);
    void stop();

signals:
    void loaded(QString savedataName, QSharedPointer<AbstractSaveData> gameInfo);
    void failedToLoad(QString savedataName);

protected:
    void run() override;

private:
    bool isStopRequested() const;
    QSharedPointer<AbstractSaveData> createSaveData(QStringView gameName
                                                  , QStringView savedataFilePath) const;

    mutable QReadWriteLock m_lock;

    bool m_isStopRequested = false;

    QStringList m_saveDataNames;
};

#endif // THREADGAMEINFOLOADER_H
