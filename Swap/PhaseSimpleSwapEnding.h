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
#ifndef PHASESIMPLESWAPENDING_H
#define PHASESIMPLESWAPENDING_H

#include "IPhase.h"
#include "fifteen/PuzzlePiece.h"

class PhaseSimpleSwapEnding : public IPhase
{
public:
    PhaseSimpleSwapEnding(QList<Fifteen::PuzzlePiecePointer> &pieces, PhaseType nextPhase);

    void click(const QPoint &) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

private:
    QList<Fifteen::PuzzlePiecePointer> &pieces;
    PhaseType nextPhase;

    int frameMoveCount;
};

#endif // PHASESIMPLESWAPENDING_H
