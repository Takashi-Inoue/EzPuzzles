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
#ifndef FIFTEENSWAPSHUFFLER_H
#define FIFTEENSWAPSHUFFLER_H

#include "FifteenIShuffler.h"
#include "IPuzzlePiece.h"

#include <random>

namespace Fifteen {

class SwapShuffler : public IShuffler
{
public:
    SwapShuffler(QList<QList<PuzzlePiecePointer>> &pieces);
    ~SwapShuffler() = default;

    void shufflePieces() override;

private:
    QList<QList<PuzzlePiecePointer>> &pieces;
    mutable std::mt19937 mt;
};

} // Fifteen

#endif // FIFTEENSWAPSHUFFLER_H
