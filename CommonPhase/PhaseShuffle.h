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
#ifndef SIMPLESLIDEPHASEPREGAME_H
#define SIMPLESLIDEPHASEPREGAME_H

#include "IPhase.h"
#include "Board.h"
#include "fifteen/FifteenAbstractShuffler.h"
#include "fifteen/PuzzlePiece.h"

class PhaseShuffle : public IPhase
{
    Q_OBJECT
public:
    PhaseShuffle(BoardPointer board, Fifteen::AbstractShuffler *shuffler, PhaseType nextPhase, QObject *parent = 0);
    ~PhaseShuffle();

    void click(const QPoint &) override;
    void onTickFrame() override {}
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

private slots:
    void completeShuffling();

private:
    BoardPointer board;
    Fifteen::AbstractShuffler *shuffler;
    PhaseType nextPhase;

    bool isEnableDraw;
};

#endif // SIMPLESLIDEPHASEPREGAME_H
