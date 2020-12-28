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

#include "ThreadGameInfoLoader.h"

#include "EzPuzzles.h"
#include "MineSweeper/SaveDataMineSweeper.h"
#include "Slide/SaveDataSimpleSlide.h"
#include "Swap/SaveDataSimpleSwap.h"
#include "BrokenSaveData.h"
#include "UnknownSaveData.h"

#include <QFile>
#include <QDebug>

void ThreadGameInfoLoader::setSaveDataNames(QStringList saveDataNames)
{
    QWriteLocker locker(&m_lock);

    m_saveDataNames = saveDataNames;
}

void ThreadGameInfoLoader::stop()
{
    QWriteLocker locker(&m_lock);

    qDebug() << QStringLiteral("ThreadGameInfoLoader has received a stop request.");

    m_isStopRequested = true;
}

void ThreadGameInfoLoader::run()
{
    m_lock.lockForWrite();
    m_isStopRequested = false;
    m_lock.unlock();

    const QString savedataDir = EzPuzzles::saveDirPath() + "/";

    for (QStringView saveDataName : m_saveDataNames) {
        qInfo() << QStringLiteral("ThreadGameInfoLoader read save data. [%1]").arg(saveDataName);

        if (isStopRequested()) {
            qInfo() << QStringLiteral("ThreadGameInfoLoader stopped.");
            return;
        }

        QFile file(QStringLiteral("%1%2").arg(savedataDir, saveDataName));

        if (!file.open(QIODevice::ReadOnly)) {
            emit failedToLoad(saveDataName.toString());
            continue;
        }

        QDataStream stream(&file);
        QString gameName;

        stream >> gameName;

        QSharedPointer<AbstractSaveData> gameInfo = createSaveData(gameName, file.fileName());

        if (!gameInfo->readInfo())
            gameInfo = QSharedPointer<BrokenSaveData>::create(file.fileName());

        emit loaded(saveDataName.toString(), gameInfo);
    }

    qInfo() << QStringLiteral("ThreadGameInfoLoader completed.");
}

bool ThreadGameInfoLoader::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}

QSharedPointer<AbstractSaveData> ThreadGameInfoLoader::createSaveData(
        QStringView gameName, QStringView savedataFilePath) const
{
    if (gameName == EzPuzzles::gameName(EzPuzzles::SimpleSlide))
        return QSharedPointer<Slide::SaveDataSimpleSlide>::create(savedataFilePath);

    if (gameName == EzPuzzles::gameName(EzPuzzles::SimpleSwap))
        return QSharedPointer<Swap::SaveDataSimpleSwap>::create(savedataFilePath);

    if (gameName == EzPuzzles::gameName(EzPuzzles::MineSweeper))
        return QSharedPointer<MineSweeper::SaveDataMineSweeper>::create(savedataFilePath);

    return QSharedPointer<UnknownSaveData>::create(savedataFilePath);
}
