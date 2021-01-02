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

namespace MineSweeper {

class PiecesFactory
{
public:
    PiecesFactory(const QPixmap &sourcePixmap, BoardInfoPointer boardInformation, int mineCount
                , bool isKeepMinesPositions);
    ~PiecesFactory() = default;

    using MinePiece2DList = QList<QList<MinePiecePointer>>;

    static QList<int> toIntList(const MinePiece2DList &pieces);
    MinePiece2DList toPieces(const QList<int> &intList);

    MinePiece2DList createPieces();
    const QList<QPoint> &minesPositions() const;

private:
    static constexpr int m_openFlag = (1 << 8);
    static constexpr int m_lockFlag = (1 << 9);
    static constexpr int m_mineID   = 0b1111;

    MinePiece2DList createNullPiecesList() const;

    void createMinePieces(MinePiece2DList &pieces);
    void createMinePiece(MinePiece2DList &pieces, int x, int y);

    void createSafePieces(MinePiece2DList &pieces) const;
    void createSafePiece(MinePiece2DList &pieces, int x, int y, int numberOfAroundMines) const;

    void createWallPieces(MinePiece2DList &pieces) const;

    QList<MinePiecePointer> getAroundPieces(const MinePiece2DList &pieces, int x, int y) const;

    QPixmap m_sourcePixmap;
    BoardInfoPointer m_boardInformation;
    int m_mineCount;
    bool m_isKeepMinesPositions;

    QList<QPoint> m_minesPositions;
};

} // MineSweeper

#endif // MINEPIECESFACTORY_H
