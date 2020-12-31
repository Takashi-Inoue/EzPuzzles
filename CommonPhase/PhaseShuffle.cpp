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

PhaseShuffle::PhaseShuffle(BoardPointer board, ShufflerPtr shuffler
                         , PhaseType nextPhaseType, QObject *parent)
    : IPhase(parent)
    , m_board(board)
    , m_shuffler(shuffler)
    , m_nextPhaseType(nextPhaseType)
    , m_isEnableDraw(true)
{
    shuffler->moveToThread(&gameThread);

    connect(shuffler.get(), SIGNAL(completed()), this, SLOT(onShufflingCompleted()));

    if (!gameThread.isRunning())
        gameThread.start();

    shuffler->start();
}

void PhaseShuffle::click(const QPoint &)
{
    m_isEnableDraw = false;
}

void PhaseShuffle::draw(QPainter &painter)
{
    if (m_isEnableDraw)
        m_board->draw(painter);
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
    return QStringLiteral("Shuffling...");
}

void PhaseShuffle::onShufflingCompleted()
{
    emit toNextPhase(m_nextPhaseType);
}
