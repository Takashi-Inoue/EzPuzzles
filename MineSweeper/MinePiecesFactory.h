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
#ifndef MINEPIECESFACTORY_H
#define MINEPIECESFACTORY_H

#include "IMinePiece.h"
#include "BoardInformation.h"
#include <QList>
#include <QPixmap>

#include <memory>
#include <random>

namespace MineSweeper {

class PiecesFactory
{
public:
    PiecesFactory(const QPixmap &sourcePixmap, BoardInfoPointer boardInformation, int mineCount, bool isKeepMinesPositions);
    ~PiecesFactory() = default;

    static QList<int> toIntList(const QVector<QVector<MinePiecePointer>> &pieces);
    QVector<QVector<MinePiecePointer>> toPieces(const QList<int> &intList);

    QVector<QVector<MinePiecePointer>> createPieces();
    const QList<QPoint> &getMinesPositions() const;

private:
    static const int openFlag = (1 << 8);
    static const int lockFlag = (1 << 9);
    static const int mineID = 9;

    void fillWithNull(QVector<QVector<MinePiecePointer>> &pieces) const;
    void createWallPieces(QVector<QVector<MinePiecePointer>> &pieces) const;
    void createMinePieces(QVector<QVector<MinePiecePointer>> &pieces);
    void createMinePieces(QVector<QVector<MinePiecePointer>> &pieces, int minX, int maxX, int minY, int maxY, int numberOfMines);
    void createSafePieces(QVector<QVector<MinePiecePointer>> &pieces) const;

    QList<MinePiecePointer> getAroundPieces(const QVector<QVector<MinePiecePointer>> &pieces, int x, int y) const;
    void createMinePiece(QVector<QVector<MinePiecePointer>> &pieces, int x, int y);
    void createSafePiece(QVector<QVector<MinePiecePointer>> &pieces, int x, int y, int numberOfAroundMines) const;

    const QPixmap &sourcePixmap;
    BoardInfoPointer boardInformation;
    int mineCount;
    bool isKeepMinesPositions;

    QList<QPoint> minesPositions;

    std::mt19937 mt;
};

} // MineSweeper

#endif // MINEPIECESFACTORY_H
