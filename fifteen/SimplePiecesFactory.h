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
#ifndef SIMPLEPIECESFACTORY_H
#define SIMPLEPIECESFACTORY_H

#include "IPiecesFactory.h"
#include <QPixmap>

namespace Fifteen {

class SimplePiecesFactory : public IPiecesFactory
{
public:
    static PuzzlePiecePointer createPiece(const QPoint &posInArray, const QPixmap &pixmap);

    SimplePiecesFactory(const QPixmap &sourcePixmap, const QSize &xy);
    ~SimplePiecesFactory() = default;

    // IPiecesFactory
    QList<QList<PuzzlePiecePointer>> createPieces() const override;

private:
    QPixmap sourcePixmap;

    QList<double> splitterXs;
    QList<double> splitterYs;
};

} // Fifteen

#endif // SIMPLEPIECESFACTORY_H
