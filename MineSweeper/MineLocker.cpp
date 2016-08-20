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
#include "MineLocker.h"
#include "Utility.h"

namespace MineSweeper {

MineLocker::MineLocker(QVector<QVector<MinePiecePointer>> &pieces) :
    pieces(pieces)
{
}

void MineLocker::addMinePos(const QPoint &minePos)
{
    notLockedMinesPos << minePos;
}

void MineLocker::addMinesPositions(const QList<QPoint> &minesPositions)
{
    notLockedMinesPos += minesPositions;
}

void MineLocker::setMinesPositions(const QList<QPoint> &minesPositions)
{
    notLockedMinesPos = minesPositions;
}

QList<QPoint> MineLocker::lockMines()
{
    QList<QPoint> result;

    for (int i = 0, max = notLockedMinesPos.size(); i < max; ++i) {
        auto &mine = getPiece(notLockedMinesPos.at(i));

        if (mine->isLock()) {
            notLockedMinesPos.removeAt(i--);
            --max;

            continue;
        }

        result += checkMinesForLock(notLockedMinesPos.at(i));

        if (mine->isLock()) {
            notLockedMinesPos.removeAt(i--);
            --max;
        }
    }

    return result;
}

QList<QPoint> MineLocker::checkMinesForLock(const QPoint &minePos)
{
    QList<QPoint> result;

    auto openedPiecesPos = getAroundOpenedPositions(minePos);

    for (const auto &openedPos : openedPiecesPos) {
        if (getPiece(openedPos)->numberOfAroundMines() > 0)
            result += checkAroundMinesForLock(openedPos);
    }

    return result;
}

QList<QPoint> MineLocker::checkAroundMinesForLock(const QPoint &openedNearMinePos)
{
    QList<QPoint> minesPosForLock;
    int numberOfOpened = 0;

    auto aroundPositions = Utility::getAroundPositions(openedNearMinePos);

    for (auto &aroundPos : aroundPositions) {
        auto &piece = getPiece(aroundPos);

        if (piece->isMine()) {
            if (!piece->isLock())
                minesPosForLock << aroundPos;
        } else {
            if (piece->isOpen())
                ++numberOfOpened;
        }
    }

    if (numberOfOpened != 8 - getPiece(openedNearMinePos)->numberOfAroundMines())
        return QList<QPoint>();

    for (auto &minePos : minesPosForLock)
        getPiece(minePos)->lock();

    return minesPosForLock;
}

MinePiecePointer &MineLocker::getPiece(const QPoint &pos)
{
    return pieces[pos.y()][pos.x()];
}

const MinePiecePointer &MineLocker::getPiece(const QPoint &pos) const
{
    return pieces.at(pos.y()).at(pos.x());
}

QList<QPoint> MineLocker::getAroundOpenedPositions(const QPoint &pos) const
{
    QList<QPoint> result = Utility::getAroundPositions(pos);

    auto itr = std::remove_if(result.begin(), result.end(), [&](const QPoint &aroundPos) {
        auto &piece = getPiece(aroundPos);

        return piece->isMine() || !piece->isOpen();
    });

    result.erase(itr, result.end());

    return result;
}

} // MineSweeper
