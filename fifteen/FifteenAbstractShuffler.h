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
#ifndef FIFTEENABSTRACTSHUFFLER_H
#define FIFTEENABSTRACTSHUFFLER_H

#include "BoardInformation.h"
#include "IPuzzlePiece.h"

#include <QList>

namespace Fifteen {

class AbstractShuffler
{
public:
    explicit AbstractShuffler(QList<FifteenPiecePointer> &pieces, BoardInfoPointer boardInfo)
        : m_pieces(pieces)
        , m_boardInfo(boardInfo)
    {
    }

    virtual ~AbstractShuffler() = default;

    virtual void exec() = 0;

protected:
    QList<FifteenPiecePointer> &m_pieces;
    BoardInfoPointer m_boardInfo;
};

} // Fifteen

using ShufflerPtr = QSharedPointer<Fifteen::AbstractShuffler>;

#endif // FIFTEENABSTRACTSHUFFLER_H
