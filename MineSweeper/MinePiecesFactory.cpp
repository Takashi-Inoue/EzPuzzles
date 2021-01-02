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
#include "MinePiecesFactory.h"
#include "MinePiece.h"
#include "SafePiece.h"
#include "WallPiece.h"

#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QDebug>

namespace MineSweeper {

PiecesFactory::PiecesFactory(const QPixmap &sourcePixmap, BoardInfoPointer boardInformation
                           , int mineCount, bool isKeepMinesPositions)
    : m_sourcePixmap(sourcePixmap)
    , m_boardInformation(boardInformation)
    , m_mineCount(mineCount)
    , m_isKeepMinesPositions(isKeepMinesPositions)
{
    Q_ASSERT(!sourcePixmap.isNull());
    Q_ASSERT(mineCount > 0 && mineCount < boardInformation->pieceCount());
}

QList<int> PiecesFactory::toIntList(const MinePiece2DList &pieces)
{
    Q_ASSERT(!pieces.isEmpty());

    QList<int> result;

    qsizetype xMax = pieces[0].size() - 1;
    qsizetype yMax = pieces.size() - 1;

    for (int y = 1; y < yMax; ++y) {
        for (int x = 1; x < xMax; ++x) {
            const MinePiecePointer &piece = pieces[y][x];

            int value = piece->isMine() ? m_mineID : piece->numberOfAroundMines();

            if (piece->isLock())
                value |= m_lockFlag;

            result << (piece->isOpen() ? (value | m_openFlag) : value);
        }
    }

    return result;
}

PiecesFactory::MinePiece2DList PiecesFactory::toPieces(const QList<int> &intList)
{
    m_minesPositions.clear();

    MinePiece2DList pieces = createNullPiecesList();

    createWallPieces(pieces);

    for (int i = 0, lim = int(intList.size()); i < lim; ++i) {
        const int num = intList[i];
        const int x = i % m_boardInformation->xCount() + 1;
        const int y = i / m_boardInformation->xCount() + 1;

        if ((num & m_mineID) > 8) { // 8 == Max mines around
            createMinePiece(pieces, x, y);
            m_minesPositions << QPoint(x, y);
        } else {
            createSafePiece(pieces, x, y, num & m_mineID);
        }

        if (num & m_openFlag)
            pieces[y][x]->open();

        if (num & m_lockFlag)
            pieces[y][x]->lock();
    }

    return pieces;
}

PiecesFactory::MinePiece2DList PiecesFactory::createPieces()
{
    MinePiece2DList pieces = createNullPiecesList();

    createWallPieces(pieces);
    createMinePieces(pieces);
    createSafePieces(pieces);

    return pieces;
}

const QList<QPoint> &PiecesFactory::minesPositions() const
{
    return m_minesPositions;
}

PiecesFactory::MinePiece2DList PiecesFactory::createNullPiecesList() const
{
    int cx = m_boardInformation->xCount() + 2;
    int cy = m_boardInformation->yCount() + 2;

    MinePiece2DList pieces(cy, QList<MinePiecePointer>(cx, nullptr));

    return pieces;
}

void PiecesFactory::createMinePieces(MinePiece2DList &pieces)
{
    Q_ASSERT(pieces.size() == m_boardInformation->yCount() + 2);

    QElapsedTimer timer;
    timer.start();

    const int xCount = m_boardInformation->xCount();
    const int yCount = m_boardInformation->yCount();
    const int numberOfPieces = xCount * yCount;

    QList<int> indices(numberOfPieces);

    std::iota(indices.begin(), indices.end(), 0);

    for (int i = numberOfPieces - 1; i >= 0; --i) {
        int randIndex = QRandomGenerator::global()->bounded(i + 1);
        int index = indices[randIndex];
        std::swap(indices[randIndex], indices[i]);

        int x = index % xCount + 1;
        int y = index / xCount + 1;

        Q_ASSERT(pieces[y][x] == nullptr);

        createMinePiece(pieces, x, y);

        if (m_mineCount + i == numberOfPieces)
            break;
    }

    qDebug() << "create Mine pieces" << timer.elapsed() << "ms";
}

void PiecesFactory::createMinePiece(MinePiece2DList &pieces, int x, int y)
{
    auto minePiece = QSharedPointer<MinePiece>::create(
                         m_boardInformation->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect());

    pieces[y][x] = minePiece;

    if (m_isKeepMinesPositions)
        m_minesPositions << QPoint(x, y);
}

void PiecesFactory::createSafePieces(MinePiece2DList &pieces) const
{
    QElapsedTimer timer;
    timer.start();

    int cx = m_boardInformation->xCount() + 2;
    int cy = m_boardInformation->yCount() + 2;

    Q_ASSERT(pieces.size() == cy);

    for (int y = 1; y < cy - 1; ++y) {
        for (int x = 1; x < cx - 1; ++x) {
            if (pieces[y][x] != nullptr)
                continue;

            QList<MinePiecePointer> aroundPieces = getAroundPieces(pieces, x, y);

            auto numberOfAroundMines = std::count_if(aroundPieces.begin(), aroundPieces.end()
                                                   , [](MinePiecePointer &piece)
            {
                return piece != nullptr ? piece->isMine() : false;
            });

            createSafePiece(pieces, x, y, int(numberOfAroundMines));
        }
    }

    qDebug() << "create Safe pieces" << timer.elapsed() << "ms";
}

void PiecesFactory::createSafePiece(MinePiece2DList &pieces, int x, int y, int numberOfAroundMines) const
{
    QRect rect = m_boardInformation->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect();

    auto safePiece = QSharedPointer<SafePiece>::create(
                         numberOfAroundMines, rect, m_sourcePixmap, rect);

    safePiece->setOpenPieceOpacity(0.5);
    pieces[y][x] = safePiece;
}

void PiecesFactory::createWallPieces(MinePiece2DList &pieces) const
{
    QElapsedTimer timer;
    timer.start();

    int cx = m_boardInformation->xCount() + 2;
    int cy = m_boardInformation->yCount() + 2;

    Q_ASSERT(pieces.size() == cy);

    auto wallPiece = QSharedPointer<WallPiece>::create();

    for (int x = 0; x < cx; ++x) {
        pieces[     0][x] = wallPiece;
        pieces[cy - 1][x] = wallPiece;
    }

    for (int y = 1; y < cy - 1; ++y) {
        pieces[y][     0] = wallPiece;
        pieces[y][cx - 1] = wallPiece;
    }

    qDebug() << "create Wall pieces" << timer.elapsed() << "ms";
}

QList<MinePiecePointer> PiecesFactory::getAroundPieces(const MinePiece2DList &pieces, int x, int y) const
{
    Q_ASSERT(x >= 1 && x < m_boardInformation->xCount() + 1);
    Q_ASSERT(y >= 1 && y < m_boardInformation->yCount() + 1);

    QList<MinePiecePointer> result = {
        pieces[y - 1][x - 1],
        pieces[y - 1][x    ],
        pieces[y - 1][x + 1],
        pieces[y    ][x - 1],
        pieces[y    ][x + 1],
        pieces[y + 1][x - 1],
        pieces[y + 1][x    ],
        pieces[y + 1][x + 1],
    };

    return result;
}

} // MineSweeper
