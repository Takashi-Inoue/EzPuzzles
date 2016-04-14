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
#ifndef NUMBERPIECEFACTORY_H
#define NUMBERPIECEFACTORY_H

#include <QMap>
#include <QVector>
#include <memory>

class IPiece;
class QSize;

namespace MineSweeper {

class NumberPieceFactory
{
public:
    NumberPieceFactory() = default;
    ~NumberPieceFactory() = default;

    static IPiece *getPiece(int number, const QSize &size);

private:
    typedef std::shared_ptr<IPiece> PiecePointer;
    typedef QVector<PiecePointer> PieceList;
    typedef QPair<int, int> keyPair;

    static QMap<keyPair, PieceList> piecesMap;

    NumberPieceFactory(const NumberPieceFactory &) = delete;
    NumberPieceFactory(NumberPieceFactory &&) = delete;
    NumberPieceFactory &operator=(const NumberPieceFactory &) = delete;
    NumberPieceFactory &operator=(NumberPieceFactory &&) = delete;
};

} // MineSweeper

#endif // NUMBERPIECEFACTORY_H
