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
#include "PhaseShuffle.h"
#include "fifteen/FifteenSlideShuffler.h"

#include <QThread>

extern QThread gameThread;

PhaseShuffle::PhaseShuffle(std::shared_ptr<Fifteen::AbstractShuffler> shuffler, PhaseType nextPhase, QObject *parent) :
    IPhase(parent),
    shuffler(shuffler),
    nextPhase(nextPhase)
{
    shuffler->moveToThread(&gameThread);

    connect(shuffler.get(), SIGNAL(update(QList<Fifteen::PuzzlePiecePointer>)), this, SLOT(addChangedPieces(QList<Fifteen::PuzzlePiecePointer>)));
    connect(shuffler.get(), SIGNAL(completed()), this, SLOT(completeShuffling()));

    if (!gameThread.isRunning())
        gameThread.start();

    shuffler->start();
}

PhaseShuffle::~PhaseShuffle()
{
    shuffler->disconnect();
}

void PhaseShuffle::draw(QPainter &painter)
{
    for (auto &piece : changedPieces)
        piece->draw(painter);

    changedPieces.clear();
}

bool PhaseShuffle::canSave() const
{
    return false;
}

bool PhaseShuffle::canLoad() const
{
    return false;
}

bool PhaseShuffle::save(QDataStream &) const
{
    Q_ASSERT(false);
    return false;
}

bool PhaseShuffle::load(QDataStream &)
{
    Q_ASSERT(false);
    return false;
}

QString PhaseShuffle::information() const
{
    return "Shuffling...";
}

void PhaseShuffle::addChangedPieces(QList<Fifteen::PuzzlePiecePointer> changed)
{
    changedPieces += changed;

    qSort(changedPieces);

    changedPieces.erase(std::unique(changedPieces.begin(), changedPieces.end()), changedPieces.end());
}

void PhaseShuffle::completeShuffling()
{
    emit toNextPhase(nextPhase);
}
