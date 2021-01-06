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
#ifndef NUMBERPIECEFACTORY_H
#define NUMBERPIECEFACTORY_H

#include <QHash>
#include <QList>
#include <QSharedPointer>

class IPiece;
class QSize;

namespace MineSweeper {

class NumberPieceFactory
{
    NumberPieceFactory(const NumberPieceFactory &) = delete;
    NumberPieceFactory(NumberPieceFactory &&) = delete;
    NumberPieceFactory &operator=(const NumberPieceFactory &) = delete;
    NumberPieceFactory &operator=(NumberPieceFactory &&) = delete;
public:
    NumberPieceFactory() = default;
    ~NumberPieceFactory() = default;

    static QSharedPointer<IPiece> getPiece(int number, const QSize &size);

private:
    using PiecePointer =QSharedPointer<IPiece>;
    using PieceList = QList<PiecePointer>;

    static QHash<QSize, PieceList> m_piecesHash;
};

} // MineSweeper

#endif // NUMBERPIECEFACTORY_H
