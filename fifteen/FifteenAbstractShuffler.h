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
#ifndef FIFTEENABSTRACTSHUFFLER_H
#define FIFTEENABSTRACTSHUFFLER_H

#include "ThreadOperation.h"
#include "BoardInformation.h"
#include "IPuzzlePiece.h"

#include <QList>
#include <QPoint>
#include <QReadWriteLock>
#include <memory>

namespace Fifteen {

class AbstractShuffler : public ThreadOperation
{
    Q_OBJECT
public:
    explicit AbstractShuffler(QList<FifteenPiecePointer> &pieces, BoardInfoPointer boardInfo, std::shared_ptr<QReadWriteLock> rwlockForPieces, QObject *parent = 0) :
        ThreadOperation(parent),
        pieces(pieces),
        boardInfo(boardInfo),
        rwlock(rwlockForPieces)
    {
        Q_CHECK_PTR(rwlockForPieces);
    }

    virtual ~AbstractShuffler() = default;

protected:
    QList<FifteenPiecePointer> &pieces;
    BoardInfoPointer boardInfo;

    std::shared_ptr<QReadWriteLock> rwlock;
};

} // Fifteen

using ShufflerPtr = QSharedPointer<Fifteen::AbstractShuffler>;

#endif // FIFTEENABSTRACTSHUFFLER_H
