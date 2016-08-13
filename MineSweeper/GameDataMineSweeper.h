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
#ifndef GAMEDATAMINESWEEPER_H
#define GAMEDATAMINESWEEPER_H

#include "IGameData.h"
#include "mine/MinePiece.h"
#include "mine/MineLocker.h"

namespace MineSweeper {

class GameDataMineSweeper : public IGameData
{
public:
    GameDataMineSweeper(const SourceImage &sourceImage, const QSize &xyCount, int mineCount, bool isAutoLock);

    QString gameName() const override;
    PhasePointer createPhase(IPhase::PhaseType) override;
    PhasePointer createCurrentPhase() override;
    const SourceImage &sourceImage() const override;
    BoardInfoPointer boardInfo() const override;

    bool save(const QString &fileName) const override;
    bool load(const QString &fileName) override;

private:
    SourceImage sourceImg;
    BoardInfoPointer boardInformation;
    int mineCount;
    QVector<QVector<MinePiecePointer>> pieces;
    MineLockerPointer mineLocker;
    PhasePointer currentPhase;
};

} // MineSweeper

#endif // GAMEDATAMINESWEEPER_H
