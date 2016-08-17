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
#include "PhaseMineSweeperGaming.h"
#include "mine/MineLocker.h"

namespace MineSweeper {

PhaseMineSweeperGaming::PhaseMineSweeperGaming(MineFieldPointer mineField, QVector<QVector<MinePiecePointer>> &pieces, PhaseType nextPhase) :
    mineField(mineField),
    pieces(pieces),
    nextPhase(nextPhase)
{
    Q_CHECK_PTR(mineField);
}

void PhaseMineSweeperGaming::click(const QPoint &clickedPiecePos)
{
    mineField->open(clickedPiecePos);

    double opacity = mineField->openedRate() / 2.0 + 0.5;

    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal) {
            if (!piece->isNearMine() && piece->isOpen())
                piece->setOpenPieceOpacity(opacity);
        }
    }

    if (mineField->isAllOpened())
        emit toNextPhase(nextPhase);
}

void PhaseMineSweeperGaming::onTickFrame()
{
//    for (auto &horizontal : pieces) {
//        for (auto &piece : horizontal) {
//            piece->onTickFrame();
//        }
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
    return "";
}

} // MineSweeper
