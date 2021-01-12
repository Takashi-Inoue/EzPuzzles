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
#ifndef GAMEDATAMINESWEEPER_H
#define GAMEDATAMINESWEEPER_H

#include "IGameData.h"
#include "MineField.h"
#include "MinePiece.h"

namespace MineSweeper {

class SaveDataMineSweeper;

class GameDataMineSweeper : public IGameData
{
public:
    GameDataMineSweeper(const SourceImage &sourceImage, const QSize &countXY, int mineCount, bool isAutoLock);
    GameDataMineSweeper(const SaveDataMineSweeper &loadedSavedata);

    // IGameData
    GameDataPointer cloneAsNewGame() const override;
    QString gameName() const override;
    PhasePointer createPhase(AbstractPhase::PhaseType) override;
    AbstractPhase::PhaseType currentPhase() const override;
    const SourceImage &sourceImage() const override;
    FinalImagePointer finalImage() const override;
    BoardInfoPointer boardInfo() const override;
    bool save(QStringView fileName) const override;

    double openedRate() const;
    QList<QPointF> explodedCenters() const;

private:
    SourceImage sourceImg;
    BoardInfoPointer boardInformation;
    int mineCount;
    AbstractPhase::PhaseType currentPhaseType;

    QVector<QVector<MinePiecePointer>> pieces;
    bool m_isAutoLock;
    MineFieldPointer mineField;
};

} // MineSweeper

#endif // GAMEDATAMINESWEEPER_H
