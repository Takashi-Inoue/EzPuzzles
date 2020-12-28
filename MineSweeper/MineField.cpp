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
#include "MineField.h"
#include "SaveDataMineSweeper.h"

namespace MineSweeper {

MineField::MineField(QVector<QVector<MinePiecePointer>> &pieces, MineLockerPointer mineLocker, int mineCount) :
    pieces(pieces),
    m_mineCount(mineCount),
    m_openedCount(0),
    m_missedCount(0),
    mineLocker(mineLocker)
{
}

void MineField::open(const QPoint &pos)
{
    Q_ASSERT(!pieces.isEmpty());

    auto &piece = pieces[pos.y()][pos.x()];

    if (piece->isOpen() | piece->isLock())
        return;

    piece->open();

    if (piece->isMine()) {
        ++m_missedCount;
        explodedPos << pos;
    } else {
        ++m_openedCount;
    }

    if (!piece->isNearMine())
        openChaining(pos);

    if (mineLocker != nullptr)
        mineLocker->lockMines();
}

void MineField::save(SaveDataMineSweeper &savedata) const
{
    savedata.m_openedCount = m_openedCount;
    savedata.m_missedCount = m_missedCount;
}

void MineField::load(const SaveDataMineSweeper &savedata)
{
    m_openedCount = savedata.m_openedCount;
    m_missedCount = savedata.m_missedCount;

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

int MineField::openedCount() const
{
    return m_openedCount;
}

int MineField::missedCount() const
{
    return m_missedCount;
}

double MineField::openedRate() const
{
    double safeCount = safePiecesCount();

    return m_openedCount / safeCount;
}

double MineField::mineRatio() const
{
    return static_cast<double>(m_mineCount) / totalPieceCount();
}

bool MineField::isAllOpened() const
{
    return safePiecesCount() == m_openedCount;
}

bool MineField::isNoMissed() const
{
    return m_missedCount == 0;
}

QString MineField::information() const
{
    double safeCount = safePiecesCount();

    return QString("%1/%2 %3% opend, %4 missed").arg(m_openedCount)
                                                .arg(safeCount)
                                                .arg(openedRate() * 100, 0, 'f', 2)
                                                .arg(m_missedCount);
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

                ++m_openedCount;

                if (!piece->isNearMine())
                    mustCheck << checkPos;
            }
        }
    }
}

int MineField::totalPieceCount() const
{
    return (pieces.size() - 2) * (pieces.first().size() - 2);
}

int MineField::safePiecesCount() const
{
    Q_ASSERT(!pieces.isEmpty());

    return totalPieceCount() - m_mineCount;
}

} // MineSweeper
