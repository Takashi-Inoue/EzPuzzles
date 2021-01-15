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
#include "Savers.h"

#include <QElapsedTimer>

namespace MineSweeper {

MineField::MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount, QObject *parent)
    : MineField(pieces, isAutoLock, mineCount, 0, 0, parent)
{
}

MineField::MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount
                   , int openedCount, int missedCount, QObject *parent)
    : QObject(parent)
    , pieces(pieces)
    , m_mineCount(mineCount)
    , m_openedCount(openedCount)
    , m_missedCount(missedCount)
    , m_isAutoLock(isAutoLock)
{
    int besideMineCount = 0;

    for (qsizetype y = 1, yLim = pieces.size() - 1; y < yLim; ++y) {
        QList<MinePiecePointer> &horizontal = pieces[y];

        for (qsizetype x = 1, xLim = horizontal.size() - 1; x < xLim; ++x) {
            const MinePiecePointer &piece = horizontal[x];

            if (!piece->isMine() && piece->isNearMine())
                ++besideMineCount;
        }
    }

    const uchar maxSavers = uchar(mineRatio() * 10 + m_mineCount / 500.0);
    const ushort requiredPoints = (maxSavers == 0)
                                  ? 0 : ushort((besideMineCount * 0.3) / (maxSavers + 1));

    m_savers = QSharedPointer<Savers>::create(maxSavers, requiredPoints);

    if (m_openedCount > 0)
        setOpenedPieceOpacity();
}

void MineField::draw(QPainter &painter)
{
    for (qsizetype y = 1, yLim = pieces.size() - 1; y < yLim; ++y) {
        QList<MinePiecePointer> &horizontal = pieces[y];

        for (qsizetype x = 1, xLim = horizontal.size() - 1; x < xLim; ++x)
            horizontal[x]->draw(painter);
    }
}

void MineField::open(const QPoint &pos)
{
    Q_ASSERT(!pieces.isEmpty());

    if ((pos.y() <= 0 || pieces.size()    - 1 <= pos.y())
     || (pos.x() <= 0 || pieces[0].size() - 1 <= pos.x()))
    {
        return;
    }

    MinePiecePointer &piece = pieces[pos.y()][pos.x()];

    if (piece->isLocked())
        return;

    if (piece->isOpened()) {
        if (piece->isMine() && m_savers->hasSaver()) {
            piece->lock();
            m_savers->consume();
            --m_missedCount;
        }

        return;
    }

    QList<QPoint> openedPointsNearMines;

    piece->open();

    if (piece->isMine()) {
        if (m_savers->hasSaver()) {
            piece->lock();
            m_savers->consume();
        } else {
            ++m_missedCount;
            m_savers->clear();
        }

        return;
    }

    ++m_openedCount;
    m_savers->incrementPoint();

    if (!piece->isNearMine()) {
        openChaining(pos, openedPointsNearMines);
    } else if (m_isAutoLock) {
        openedPointsNearMines << pos;
        gatherAroundPointsToLock(pos, openedPointsNearMines);
    }

    emit openedRateChanged(openedRate());

    lockMines(openedPointsNearMines);
    setOpenedPieceOpacity();
}

void MineField::press(const QPoint &pos)
{
    Q_ASSERT(!pieces.isEmpty());

    if ((pos.y() <= 0 || pieces.size()    - 1 <= pos.y())
     || (pos.x() <= 0 || pieces[0].size() - 1 <= pos.x()))
    {
        return;
    }

    pieces[pos.y()][pos.x()]->press();
}

void MineField::release(const QPoint &pos)
{
    Q_ASSERT(!pieces.isEmpty());

    if ((pos.y() <= 0 || pieces.size()    - 1 <= pos.y())
     || (pos.x() <= 0 || pieces[0].size() - 1 <= pos.x()))
    {
        return;
    }

    pieces[pos.y()][pos.x()]->release();
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

    return QString("%1/%2 %3% opend, %4 missed | %5")
            .arg(m_openedCount).arg(safeCount).arg(openedRate() * 100, 0, 'f', 2)
            .arg(m_missedCount)
            .arg(m_savers->information());
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
    QList<QPoint> minePositions;
    int closedPieceCount = 8;

    for (MinePiecePointer &aroundPiece : aroundPieces) {
        if (aroundPiece->isMine())
            mines << aroundPiece;
        else if (aroundPiece->isOpened())
            --closedPieceCount;
    }

    if (closedPieceCount == mines.size()) {
        for (MinePiecePointer &mine : mines) {
            if (!mine->isOpened() && !mine->isLocked())
                mine->lock();
        }
    }
}

void MineField::openChaining(const QPoint &pos, QList<QPoint> &openedPointsNearMines)
{
    QList<QPoint> centerPointsToCheck = {pos};

    while (!centerPointsToCheck.isEmpty()) {
        QPoint centerPos = centerPointsToCheck.takeFirst();

        for (const QPoint &pos : aroundPos(centerPos)) {
            MinePiecePointer &piece = pieces[pos.y()][pos.x()];

            if (piece->isMine()) {
                continue;
            } else if (piece->isOpened()) {
                if (piece->isNearMine()) {
                    openedPointsNearMines << pos;
                    gatherAroundPointsToLock(pos, openedPointsNearMines);
                }

                continue;
            }

            piece->open();

            ++m_openedCount;

            if (!piece->isNearMine()) {
                centerPointsToCheck << pos;
            } else if (m_isAutoLock) {
                openedPointsNearMines << pos;
                gatherAroundPointsToLock(pos, openedPointsNearMines);
            }
        }
    }
}

void MineField::setOpenedPieceOpacity()
{
    double opacity = openedRate() / 2.0 + 0.5;

    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->setOpenPieceOpacity(opacity);
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

        if (piece->isOpened() & piece->isNearMine() & !piece->isMine())
            openedPointsNearMines << aroundPoint;
    }
}

} // MineSweeper
