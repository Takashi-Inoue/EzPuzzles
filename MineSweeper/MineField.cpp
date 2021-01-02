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

#include <QElapsedTimer>

namespace MineSweeper {

MineField::MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount)
    : MineField(pieces, isAutoLock, mineCount, 0, 0)
{
}

MineField::MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount
                   , int openedCount, int missedCount)
    : pieces(pieces)
    , m_mineCount(mineCount)
    , m_openedCount(openedCount)
    , m_missedCount(missedCount)
    , m_isAutoLock(isAutoLock)
{
    if (m_missedCount <= 0)
        return;

    for (qsizetype y = 0, ylim = pieces.size(); y < ylim; ++y) {
        const QList<MinePiecePointer> &horizontal = pieces[y];

        for (qsizetype x = 0, xlim = horizontal.size(); x < xlim; ++x) {
            const MinePiecePointer &piece = horizontal[x];

            if (piece->isMine() && piece->isOpen())
                explodedPos << QPoint(int(x), int(y));
        }
    }
}

void MineField::open(const QPoint &pos)
{
    Q_ASSERT(!pieces.isEmpty());

    MinePiecePointer &piece = pieces[pos.y()][pos.x()];

    if (piece->isOpen() | piece->isLock())
        return;

    QList<QPoint> openedPointsNearMines;

    piece->open();

    if (piece->isMine()) {
        ++m_missedCount;
        explodedPos << pos;
    } else {
        ++m_openedCount;
    }

    if (!piece->isNearMine()) {
        openChaining(pos, openedPointsNearMines);
    } else if (m_isAutoLock) {
        openedPointsNearMines << pos;
        gatherAroundPointsToLock(pos, openedPointsNearMines);
    }

    lockMines(openedPointsNearMines);
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
    return double(m_mineCount) / totalPieceCount();
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

void MineField::lockMines(QList<QPoint> &pointsToCheckToLock)
{
    if (!m_isAutoLock)
        return;

//    QElapsedTimer timer;
//    timer.start();

    std::sort(pointsToCheckToLock.begin(), pointsToCheckToLock.end()
            , [](const QPoint &lhs, const QPoint &rhs) {
        if (lhs.x() != rhs.x())
            return lhs.x() < rhs.x();

        return lhs.y() < rhs.y();
    });

    auto itr = std::unique(pointsToCheckToLock.begin(), pointsToCheckToLock.end());
    pointsToCheckToLock.erase(itr, pointsToCheckToLock.end());

    for (const QPoint &centerPos : pointsToCheckToLock)
        lockMinesInAround(centerPos.x(), centerPos.y());

//    qDebug() << QStringLiteral("lockMines: %1ms").arg(timer.elapsed()) << pointsToCheckToLock.size();
}

void MineField::lockMinesInAround(int x, int y)
{
    QList<MinePiecePointer> aroundPieces = {
        pieces[y - 1][x - 1], pieces[y - 1][x    ], pieces[y - 1][x + 1],
        pieces[y    ][x - 1],                       pieces[y    ][x + 1],
        pieces[y + 1][x - 1], pieces[y + 1][x    ], pieces[y + 1][x + 1],
    };

    QList<MinePiecePointer> mines;
    int closedPieceCount = 8;

    for (MinePiecePointer &aroundPiece : aroundPieces) {
        if (aroundPiece->isMine())
            mines << aroundPiece;
        else if (aroundPiece->isOpen())
            --closedPieceCount;
    }

    if (closedPieceCount == mines.size()) {
        for (MinePiecePointer &mine : mines)
            mine->lock();
    }
}

void MineField::openChaining(const QPoint &pos, QList<QPoint> &openedPointsNearMines)
{
    QList<QPoint> mustCheck = {pos};

    while (!mustCheck.isEmpty()) {
        QPoint centerPos = mustCheck.takeFirst();
        QList<QPoint> positions = aroundPos(centerPos);

        for (const QPoint &pos : positions) {
            MinePiecePointer &piece = pieces[pos.y()][pos.x()];

            if (piece->isOpen() || piece->isMine())
                continue;

            piece->open();

            ++m_openedCount;

            if (!piece->isNearMine()) {
                mustCheck << pos;
            } else if (m_isAutoLock) {
                openedPointsNearMines << pos;
                gatherAroundPointsToLock(pos, openedPointsNearMines);
            }
        }
    }
}

int MineField::totalPieceCount() const
{
    Q_ASSERT(!pieces.isEmpty());

    return int((pieces.size() - 2) * (pieces[0].size() - 2));
}

int MineField::safePiecesCount() const
{
    Q_ASSERT(!pieces.isEmpty());

    return totalPieceCount() - m_mineCount;
}

QList<QPoint> MineField::aroundPos(const QPoint &centerPos) const
{
    const int x = centerPos.x();
    const int y = centerPos.y();

    return {
        QPoint(x - 1, y - 1),
        QPoint(x    , y - 1),
        QPoint(x + 1, y - 1),
        QPoint(x - 1, y    ),
        QPoint(x + 1, y    ),
        QPoint(x - 1, y + 1),
        QPoint(x    , y + 1),
        QPoint(x + 1, y + 1),
    };
}

void MineField::gatherAroundPointsToLock(const QPoint &centerPos
                                       , QList<QPoint> &openedPointsNearMines) const
{
    for (const QPoint &aroundPoint : aroundPos(centerPos)) {
        auto &piece = pieces[aroundPoint.y()][aroundPoint.x()];

        if (piece->isOpen() & piece->isNearMine() & !piece->isMine())
            openedPointsNearMines << aroundPoint;
    }
}

} // MineSweeper
