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
#ifndef IPUZZLEPIECE_H
#define IPUZZLEPIECE_H

#include "IPiece.h"
#include <memory>

class QDataStream;

namespace Fifteen {

class IPuzzlePiece : public IPiece
{
public:
    IPuzzlePiece() = default;
    virtual ~IPuzzlePiece() = default;

    virtual void setPos(const QPoint &position) = 0;
    virtual void swapPos(IPuzzlePiece *) = 0;
    virtual QPoint pos() const = 0;
    virtual bool isPosCorrect() const = 0;
};

typedef std::shared_ptr<IPuzzlePiece> PuzzlePiecePointer;

} // Fifteen

#endif // IPUZZLEPIECE_H
