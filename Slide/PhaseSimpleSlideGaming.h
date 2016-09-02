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
#ifndef PHASESIMPLESLIDEGAMING_H
#define PHASESIMPLESLIDEGAMING_H

#include "IPhase.h"
#include "Board.h"
#include "fifteen/PuzzlePiece.h"

namespace Slide {

class PhaseSimpleSlideGaming : public IPhase
{
    Q_OBJECT
public:
    PhaseSimpleSlideGaming(BoardPointer board, QPoint &currentBlankPos, const QPoint &defaultBlankPos, PhaseType nextPhase, int slideFrameCount, QObject *parent = 0);
    ~PhaseSimpleSlideGaming() = default;

    void click(const QPoint &clickedPiecePos) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

protected:
    BoardPointer board;
    QPoint &currentBlankPos;
    const QPoint defaultBlankPos;
    PhaseType nextPhase;
    int slideFrameCount;
    bool isGameCleared;
};

} // Slide

#endif // PHASESIMPLESLIDEGAMING_H
