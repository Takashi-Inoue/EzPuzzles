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
#ifndef MINESWEEPERSAVEDATA_H
#define MINESWEEPERSAVEDATA_H

#include "ISaveData.h"
#include <QSize>

namespace MineSweeper {

class MineSweeperSaveData : public ISaveData
{
public:
    MineSweeperSaveData(const QString &savedataFilePath);
    ~MineSweeperSaveData() = default;

    // IGameInfo
    QString gameName() const override;
    QString imageFilePath() const override;
    QStringList informations() const override;

    IGame *loadGame() const override;
    bool loadInfo() override;
    bool isValid() const override;
    QIcon gameTypeIcon() const override;

private:
    QString savedataFilePath;
    QString gameTypeName;
    QString imgFilePath;
    QSize xy;
    bool isAutoLock;
    int mineCount;
    int openedCount;
    int missedCount;
};

} // MineSweeper

#endif // MINESWEEPERSAVEDATA_H
