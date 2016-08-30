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
#ifndef PHASEMINESWEEPERENDING_H
#define PHASEMINESWEEPERENDING_H

#include "IPhase.h"
#include "IMinePiece.h"
#include "BoardInformation.h"
#include "SourceImage.h"

#include <random>

namespace MineSweeper {

class PhaseMineSweeperEnding : public IPhase
{
public:
    PhaseMineSweeperEnding(BoardInfoPointer boardInfo, QVector<QVector<MinePiecePointer>> &pieces, SourceImage sourceImage, PhaseType nextPhase);

    // IPhase
    void click(const QPoint &) override {}
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

private:
    void setEffectToPieces(BoardInfoPointer boardInfo);

    static const int maxWaitFrame = 60;
    static const int eraseFrames = 40;

    QVector<QVector<MinePiecePointer>> &pieces;
    SourceImage sourceImage;
    PhaseType nextPhase;

    int nowFrame;

    std::mt19937 mt;
};

} // MineSweeper

#endif // PHASEMINESWEEPERENDING_H
