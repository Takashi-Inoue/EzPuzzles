﻿/*
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

#include <QTime>
#include <QDebug>

namespace MineSweeper {

PiecesFactory::PiecesFactory(const QPixmap &sourcePixmap, BoardInfoPointer boardInformation, int mineCount, bool isKeepMinesPositions) :
    sourcePixmap(sourcePixmap),
    boardInformation(boardInformation),
    mineCount(mineCount),
    isKeepMinesPositions(isKeepMinesPositions),
    mt(std::random_device()())
{
    Q_ASSERT(!sourcePixmap.isNull());
    Q_ASSERT(mineCount > 0 && mineCount < boardInformation->pieceCount());
}

QList<int> PiecesFactory::toIntList(const QVector<QVector<MinePiecePointer>> &pieces)
{
    Q_ASSERT(!pieces.isEmpty());

    QList<int> result;

    int xMax = pieces.at(0).size() - 1;
    int yMax = pieces.size() - 1;

    for (int y = 1; y < yMax; ++y) {
        for (int x = 1; x < xMax; ++x) {
            const auto &piece = pieces.at(y).at(x);

            int value = piece->isMine() ? mineID : piece->numberOfAroundMines();

            if (piece->isLock())
                value |= lockFlag;

            result << (piece->isOpen() ? (value | openFlag) : value);
        }
    }

    return result;
}

QVector<QVector<MinePiecePointer>> PiecesFactory::toPieces(const QList<int> &intList)
{
    minesPositions.clear();

    QVector<QVector<MinePiecePointer>> pieces;

    fillWithNull(pieces);
    createWallPieces(pieces);

    for (int i = 0, lim = intList.size(); i < lim; ++i) {
        const int num = intList.at(i);
        int x = i % boardInformation->xCount() + 1;
        int y = i / boardInformation->xCount() + 1;

        if ((num & 0b1111) == mineID) {
            createMinePiece(pieces, x, y);
            minesPositions << QPoint(x, y);
        } else {
            createSafePiece(pieces, x, y, num & 0b1111);
        }

        if (num & openFlag)
            pieces[y][x]->open();

        if (num & lockFlag)
            pieces[y][x]->lock();
    }

    return pieces;
}

QVector<QVector<MinePiecePointer>> PiecesFactory::createPieces()
{
    QVector<QVector<MinePiecePointer>> pieces;

    fillWithNull(pieces);
    createWallPieces(pieces);
    createMinePieces(pieces);
    createSafePieces(pieces);

    return pieces;
}

const QList<QPoint> &PiecesFactory::getMinesPositions() const
{
    return minesPositions;
}

void PiecesFactory::fillWithNull(QVector<QVector<MinePiecePointer> > &pieces) const
{
    int cx = boardInformation->xCount() + 2;
    int cy = boardInformation->yCount() + 2;

    for (int y = 0; y < cy; ++y)
        pieces << QVector<MinePiecePointer>(cx, nullptr);
}

void PiecesFactory::createWallPieces(QVector<QVector<MinePiecePointer>> &pieces) const
{
    QTime time;
    time.start();

    int cx = boardInformation->xCount() + 2;
    int cy = boardInformation->yCount() + 2;

    Q_ASSERT(pieces.size() == cy);

    auto wallPiece = std::make_shared<WallPiece>();

    for (int x = 0; x < cx; ++x) {
        pieces[     0][x] = wallPiece;
        pieces[cy - 1][x] = wallPiece;
    }

    for (int y = 1; y < cy - 1; ++y) {
        pieces[y][     0] = wallPiece;
        pieces[y][cx - 1] = wallPiece;
    }

    qDebug() << "create Wall pieces" << time.elapsed() << "ms";
}

void PiecesFactory::createMinePieces(QVector<QVector<MinePiecePointer>> &pieces)
{
    Q_ASSERT(pieces.size() == boardInformation->yCount() + 2);

    QTime time;
    time.start();

    const int xCount = boardInformation->xCount();
    const int yCount = boardInformation->yCount();

    const int halfX = xCount / 2 + 1;
    const int halfY = yCount / 2 + 1;
    const int numOfCreatedMines = mineCount / 4;

    createMinePieces(pieces,         1, halfX,          1, halfY,  numOfCreatedMines);
    createMinePieces(pieces, halfX + 1, xCount,         1, halfY,  numOfCreatedMines);
    createMinePieces(pieces,         1, halfX,  halfY + 1, yCount, numOfCreatedMines);
    createMinePieces(pieces, halfX + 1, xCount, halfY + 1, yCount, numOfCreatedMines);

    int remian = mineCount % 4;

    for (int i = 0; i < remian; ++i) {
        int x = (mt() % xCount)  + 1;
        int y = (mt() % yCount) + 1;

        if (pieces[y][x] == nullptr)
            createMinePiece(pieces, x, y);
        else
            --i;
    }

    qDebug() << "create Mine pieces" << time.elapsed() << "ms";
}

void PiecesFactory::createMinePieces(QVector<QVector<MinePiecePointer>> &pieces, int minX, int maxX, int minY, int maxY, int numberOfMines)
{
    if (numberOfMines == 0)
        return;

    const int width  = maxX - minX + 1;
    const int height = maxY - minY + 1;
    const int numberOfPieces = width * height;


    QList<int> indexes;
    indexes.reserve(numberOfPieces);

    for (int i = 0; i < numberOfPieces; ++i)
        indexes << i;

    for (int i = numberOfPieces - 1; i >= 0; --i) {
        int randIndex = mt() % (i + 1);
        int index = indexes[randIndex];
        std::swap(indexes[randIndex], indexes[i]);

        int x = index % width + minX;
        int y = index / width + minY;

        Q_ASSERT(pieces[y][x] == nullptr);

        createMinePiece(pieces, x, y);

        if (numberOfMines + i == numberOfPieces)
            break;
    }
}

void PiecesFactory::createSafePieces(QVector<QVector<MinePiecePointer>> &pieces) const
{
    QTime time;
    time.start();

    int cx = boardInformation->xCount() + 2;
    int cy = boardInformation->yCount() + 2;

    Q_ASSERT(pieces.size() == cy);

    for (int y = 1; y < cy - 1; ++y) {
        for (int x = 1; x < cx - 1; ++x) {
            if (pieces[y][x] != nullptr)
                continue;

            auto arounds = getAroundPieces(pieces, x, y);

            int numberOfAroundMines = std::count_if(arounds.begin(), arounds.end(), [](MinePiecePointer &piece) {
                return piece != nullptr ? piece->isMine()
                                        : false;
            });

            createSafePiece(pieces, x, y, numberOfAroundMines);
        }
    }

    qDebug() << "create Safe pieces" << time.elapsed() << "ms";
}

QList<MinePiecePointer> PiecesFactory::getAroundPieces(const QVector<QVector<MinePiecePointer>> &pieces, int x, int y) const
{
    Q_ASSERT(x >= 1 && x < boardInformation->xCount() + 1);
    Q_ASSERT(y >= 1 && y < boardInformation->yCount() + 1);

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

void PiecesFactory::createMinePiece(QVector<QVector<MinePiecePointer>> &pieces, int x, int y)
{
    auto minePiece = std::make_shared<MinePiece>(boardInformation->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect());

    pieces[y][x] = minePiece;

    if (isKeepMinesPositions)
        minesPositions << QPoint(x, y);
}

void PiecesFactory::createSafePiece(QVector<QVector<MinePiecePointer> > &pieces, int x, int y, int numberOfAroundMines) const
{
    QRect rect = boardInformation->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect();

    auto safePiece = std::make_shared<SafePiece>(numberOfAroundMines, rect, sourcePixmap, rect);

    safePiece->setOpenPieceOpacity(0.5);
    pieces[y][x] = safePiece;
}

} // MineSweeper