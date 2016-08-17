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
#include "MineField.h"
#include "SaveDataMineSweeper.h"

namespace MineSweeper {

MineField::MineField(QVector<QVector<MinePiecePointer>> &pieces, MineLockerPointer mineLocker, int mineCount) :
    pieces(pieces),
    mineCount(mineCount),
    openedCount(0),
    missedCount(0),
    mineLocker(mineLocker)
{
    Q_ASSERT(!pieces.isEmpty());
}

void MineField::open(const QPoint &pos)
{
    auto &piece = pieces[pos.y()][pos.x()];

    piece->open();

    if (piece->isMine()) {
        ++missedCount;
        explodedPos << pos;
    } else {
        ++openedCount;
    }

    if (!piece->isNearMine())
        openChaining(pos);

    if (mineLocker != nullptr)
        mineLocker->lockMines();
}

void MineField::save(SaveDataMineSweeper &savedata) const
{
    savedata.openedCount = openedCount;
    savedata.missedCount = missedCount;
}

void MineField::load(const SaveDataMineSweeper &savedata)
{
    openedCount = savedata.openedCount;
    missedCount = savedata.missedCount;

    explodedPos.clear();

    for (int y = 0, ylim = pieces.size(); y < ylim; ++y) {
        const auto &horizontal = pieces.at(y);

        for (int x = 0, xlim = horizontal.size(); x < xlim; ++x) {
            const auto &piece = horizontal.at(x);

            if (piece->isMine() & piece->isOpen())
                explodedPos << QPoint(x, y);
        }
    }
}

double MineField::openedRate() const
{
    Q_ASSERT(!pieces.isEmpty());

    double safePieceCount = pieces.size() * pieces.first().size();

    return openedCount / safePieceCount;
}

bool MineField::isAllOpened() const
{
    Q_ASSERT(!pieces.isEmpty());

    return pieces.size() * pieces.first().size() == openedCount;
}

const QList<QPoint> &MineField::explodedPositions() const
{
    return explodedPos;
}

void MineField::openChaining(const QPoint &pos)
{
    QList<QPoint> mustCheck = {pos};

    while (!mustCheck.isEmpty()) {
        QPoint center = mustCheck.takeFirst();

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if ((dx | dy) == 0)
                    continue;

                QPoint checkPos(center + QPoint(dx, dy));

                auto &piece = pieces[checkPos.y()][checkPos.x()];

                if (piece->isOpen() || piece->isMine())
                    continue;

                piece->open();

                ++openedCount;

                if (!piece->isNearMine())
                    mustCheck << checkPos;
            }
        }
    }
}

} // MineSweeper
