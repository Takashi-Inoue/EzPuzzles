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
#include "PhaseMineSweeperGaming.h"

namespace MineSweeper {

PhaseMineSweeperGaming::PhaseMineSweeperGaming(MineFieldPointer mineField, QVector<QVector<MinePiecePointer>> &pieces, PhaseType nextPhase) :
    mineField(mineField),
    pieces(pieces),
    nextPhase(nextPhase)
{
    Q_CHECK_PTR(mineField);

    setOpenPieceOpacity();
}

void PhaseMineSweeperGaming::click(const QPoint &clickedPiecePos)
{
    if (mineField->isAllOpened() & !mineField->isNoMissed()) {
        emit toNextPhase(IPhase::PhaseReady);
        return;
    }

    mineField->open(clickedPiecePos);

    setOpenPieceOpacity();

    if (mineField->isAllOpened() & mineField->isNoMissed())
        emit toNextPhase(nextPhase);
}

void PhaseMineSweeperGaming::onTickFrame()
{
//    for (auto &horizontal : pieces) {
//        for (auto &piece : horizontal)
//            piece->onTickFrame();
//    }
}

void PhaseMineSweeperGaming::draw(QPainter &painter)
{
    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->draw(painter);
    }
}

bool PhaseMineSweeperGaming::canSave() const
{
    return true;
}

bool PhaseMineSweeperGaming::canLoad() const
{
    return true;
}

QString PhaseMineSweeperGaming::information() const
{
    return mineField->information();
}

void PhaseMineSweeperGaming::setOpenPieceOpacity()
{
    double opacity = mineField->openedRate() / 2.0 + 0.5;

    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->setOpenPieceOpacity(opacity);
    }
}

} // MineSweeper
