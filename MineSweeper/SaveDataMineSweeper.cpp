﻿/*
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
#include "SaveDataMineSweeper.h"
#include "EzPuzzles.h"

#include <QSaveFile>

namespace MineSweeper {

SaveDataMineSweeper::SaveDataMineSweeper(const QString &fileName) :
    fileName(fileName),
    isSavedataValid(true)
{
    Q_ASSERT(!fileName.isEmpty());
}

QIcon SaveDataMineSweeper::gameTypeIcon() const
{
    return QIcon(":/ico/gameMine");
}

bool SaveDataMineSweeper::isValid() const
{
    return isSavedataValid;
}

bool SaveDataMineSweeper::loadInfo()
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        isSavedataValid = false;
        return false;
    }

    return loadInfo(QDataStream(&file));
}

QString SaveDataMineSweeper::gameName() const
{
    return gameTypeName;
}

QString SaveDataMineSweeper::imageFilePath() const
{
    return sourceImg.fullPath;
}

QStringList SaveDataMineSweeper::informations() const
{
    int cellCount = xyCount.width() * xyCount.height();
    int safePieceCount = cellCount - mineCount;

    QString openedDescription = QString("%1/%2 opened (%3%), %4 missed").arg(openedCount)
                                                                        .arg(safePieceCount)
                                                                        .arg((openedCount * 100.0) / safePieceCount, 0, 'f', 2)
                                                                        .arg(missedCount);

    return {
        QString("W%1 x H%2 : %3 cells").arg(xyCount.width()).arg(xyCount.height()).arg(cellCount),
        "",
        QString("%1 mines (%2%)").arg(mineCount).arg((mineCount * 100.0) / cellCount, 0, 'f', 2),
        openedDescription,
        "",
        QString("Autolock naked mines : ") + (isAutoLock ? "On" : "Off"),
    };
}

bool SaveDataMineSweeper::save() const
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream stream(&file);

    stream << gameTypeName;
    stream << xyCount;
    stream << mineCount;
    stream << openedCount;
    stream << missedCount;
    stream << isAutoLock;
    stream << sourceImg.fullPath;
    stream << sourceImg.pixmap;
    stream << static_cast<qint8>(currentPhaseType);
    stream << pieces;

    return file.commit();
}

bool SaveDataMineSweeper::loadInfo(QDataStream &stream)
{
    stream >> gameTypeName;

    if (gameTypeName != EzPuzzles::gameName(EzPuzzles::MineSweeper)) {
        isSavedataValid = false;
        return false;
    }

    stream >> xyCount;
    stream >> mineCount;
    stream >> openedCount;
    stream >> missedCount;
    stream >> isAutoLock;
    stream >> sourceImg.fullPath;

    isSavedataValid = (stream.status() == QDataStream::Ok);

    return isSavedataValid;
}

bool SaveDataMineSweeper::load()
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        isSavedataValid = false;
        return false;
    }

    QDataStream stream(&file);

    if (!loadInfo(stream))
        return false;

    stream >> sourceImg.pixmap;

    qint8 phaseType;

    stream >> phaseType;

    currentPhaseType = static_cast<IPhase::PhaseType>(phaseType);

    stream >> pieces;

    isSavedataValid = (stream.status() == QDataStream::Ok);

    return isSavedataValid;
}

} // MineSweeper
