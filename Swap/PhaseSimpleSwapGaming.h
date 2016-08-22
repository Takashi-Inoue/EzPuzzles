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
#ifndef PHASESIMPLESWAPGAMING_H
#define PHASESIMPLESWAPGAMING_H

#include "IPhase.h"
#include "Board.h"
#include "fifteen/PuzzlePiece.h"

class PhaseSimpleSwapGaming : public IPhase
{
    Q_OBJECT
public:
    PhaseSimpleSwapGaming(BoardPointer board, QList<Fifteen::PuzzlePiecePointer> &pieces,
                          const QPoint &swapTargetPos, PhaseType nextPhase, int slideFrameCount, QObject *parent = 0);
    ~PhaseSimpleSwapGaming() = default;

    void click(const QPoint &clickedPiecePos) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

protected:
    Fifteen::PuzzlePiecePointer &getPiece(const QPoint &pos);

    BoardPointer board;
    QList<Fifteen::PuzzlePiecePointer> &pieces;
    QPoint swapTargetPos;
    PhaseType nextPhase;
    int slideFrameCount;
    bool isGameCleared;
};

#endif // PHASESIMPLESWAPGAMING_H
