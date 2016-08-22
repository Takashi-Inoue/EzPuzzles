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
#include "PhaseShuffle.h"
#include "fifteen/FifteenSlideShuffler.h"

#include <QThread>

extern QThread gameThread;

PhaseShuffle::PhaseShuffle(BoardPointer board, Fifteen::AbstractShuffler *shuffler, PhaseType nextPhase, QObject *parent) :
    IPhase(parent),
    board(board),
    shuffler(shuffler),
    nextPhase(nextPhase),
    isEnableDraw(true)
{
    shuffler->moveToThread(&gameThread);

    connect(shuffler, SIGNAL(completed()), this, SLOT(completeShuffling()));

    if (!gameThread.isRunning())
        gameThread.start();

    shuffler->start();
}

PhaseShuffle::~PhaseShuffle()
{
    shuffler->disconnect();
    delete shuffler;
}

void PhaseShuffle::click(const QPoint &)
{
    isEnableDraw = false;
}

void PhaseShuffle::draw(QPainter &painter)
{
    if (isEnableDraw)
        board->draw(painter);
}

bool PhaseShuffle::canSave() const
{
    return false;
}

bool PhaseShuffle::canLoad() const
{
    return false;
}

QString PhaseShuffle::information() const
{
    return "Shuffling...";
}

void PhaseShuffle::completeShuffling()
{
    emit toNextPhase(nextPhase);
}
