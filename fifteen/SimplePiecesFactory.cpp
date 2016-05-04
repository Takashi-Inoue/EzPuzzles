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
#include "SimplePiecesFactory.h"

#include "GridSplitter.h"
#include "PuzzlePiece.h"

namespace Fifteen {

PuzzlePiecePointer SimplePiecesFactory::createPiece(const QPoint &posInArray, const QPixmap &pixmap)
{
    return std::make_shared<PuzzlePiece>(posInArray, pixmap, pixmap.rect());
}

SimplePiecesFactory::SimplePiecesFactory(const QPixmap &sourcePixmap, const QSize &xy) :
    sourcePixmap(sourcePixmap)
{
    Q_ASSERT(!sourcePixmap.isNull());
    Q_ASSERT(!xy.isEmpty());

    splitterXs.push_back(0);
    splitterYs.push_back(0);

    splitterXs << GridSplitter::verticalSplitterPos(sourcePixmap.width(), xy.width() - 1) << sourcePixmap.width();
    splitterYs << GridSplitter::horizontalSplitterPos(sourcePixmap.height(), xy.height() - 1) << sourcePixmap.height();
}

QList<QList<PuzzlePiecePointer>> SimplePiecesFactory::createPieces() const
{
    QList<QList<PuzzlePiecePointer>> pieces;

    int ylim = splitterYs.size() - 1;
    int xlim = splitterXs.size() - 1;

    for (int y = 0; y < ylim; ++y) {
        QList<PuzzlePiecePointer> horizontal;

        for (int x = 0; x < xlim; ++x) {
            QPointF sourceTL(splitterXs.at(x    ), splitterYs.at(y    ));
            QPointF sourceBR(splitterXs.at(x + 1), splitterYs.at(y + 1));

            horizontal << std::make_shared<PuzzlePiece>(QPoint(x, y), sourcePixmap, QRectF(sourceTL, sourceBR));
        }

        pieces << horizontal;
    }

    return pieces;
}

} // Fifteen
