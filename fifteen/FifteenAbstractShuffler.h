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

#include "IPuzzlePiece.h"
#include <QObject>
#include <QList>
#include <QPoint>

namespace Fifteen {

class AbstractShuffler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractShuffler(QList<PuzzlePiecePointer> &pieces, QObject *parent = 0) :
        QObject(parent),
        pieces(pieces)
    {
        qRegisterMetaType<QList<PuzzlePiecePointer>>("QList<PuzzlePiecePointer>");
    }

    virtual ~AbstractShuffler() = default;

    virtual void shufflePieces() = 0;

signals:
    void update(QList<PuzzlePiecePointer> changed);

protected:
    QList<PuzzlePiecePointer> &pieces;
};

} // Fifteen

#endif // FIFTEENABSTRACTSHUFFLER_H
