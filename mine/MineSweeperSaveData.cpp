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

#include "MineSweeperSaveData.h"
#include "GameMineSweeper.h"

#include <QDataStream>
#include <QFile>

namespace MineSweeper {

MineSweeperSaveData::MineSweeperSaveData(const QString &savedataFilePath) :
    savedataFilePath(savedataFilePath)
{
    Q_ASSERT(!savedataFilePath.isEmpty());
}

QString MineSweeperSaveData::gameName() const
{
    return gameTypeName;
}

QString MineSweeperSaveData::imageFilePath() const
{
    return imgFilePath;
}

QStringList MineSweeperSaveData::informations() const
{
    int numberOfCells = xy.width() * xy.height();
    int safeCount = numberOfCells - mineCount;

    QString openedDescription = QString("%1/%2 opened (%3%), %4 missed").arg(openedCount)
                                                                        .arg(safeCount)
                                                                        .arg((openedCount * 100.0) / safeCount, 0, 'f', 2)
                                                                        .arg(missedCount);

    return {
        QString("W%1 x H%2 : %3 cells").arg(xy.width()).arg(xy.height()).arg(numberOfCells),
        "",
        QString("%1 mines (%2%)").arg(mineCount).arg((mineCount * 100.0) / numberOfCells, 0, 'f', 2),
        openedDescription,
        "",
        QString("Autolock naked mines : ") + (isAutoLock ? "On" : "Off"),
    };
}

IGame *MineSweeperSaveData::loadGame() const
{
    auto game = new GameMineSweeper();

    if (!game->load(savedataFilePath))
        return nullptr;

    return game;
}

bool MineSweeperSaveData::loadInfo()
{
    // GameMineSweeper::save() の書き込み順と同調すること

    QFile file(savedataFilePath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&file);

    stream >> gameTypeName;

    if (gameTypeName != GameMineSweeper::gameName())
        return false;

    stream >> imgFilePath;
    stream >> xy;
    stream >> isAutoLock;
    stream >> mineCount;
    stream >> openedCount;
    stream >> missedCount;

    if (stream.status() != QDataStream::Ok)
        return false;

    return true;
}

bool MineSweeperSaveData::isValid() const
{
    return true;
}

QIcon MineSweeperSaveData::gameTypeIcon() const
{
    return QIcon(":/ico/gameMine");
}

} // MineSweeper
