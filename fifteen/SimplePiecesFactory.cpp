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

        for (int x = 0; x < xlim; ++x)
            horizontal << createPiece(QPoint(x, y));

        pieces << horizontal;
    }

    return pieces;
}

QList<QList<PuzzlePiecePointer>> SimplePiecesFactory::createPieces(const QList<QPoint> &defaultPositions) const
{
    QList<QList<PuzzlePiecePointer>> pieces;

    int ylim = splitterYs.size() - 1;
    int xlim = splitterXs.size() - 1;

    for (int y = 0; y < ylim; ++y) {
        QList<PuzzlePiecePointer> horizontal;

        for (int x = 0; x < xlim; ++x) {
            QPoint defaultPos = defaultPositions.at(y * xlim + x);

            auto &piece = createPiece(defaultPos);

            piece->setPos(QPoint(x, y));

            horizontal << piece;
        }

        pieces << horizontal;
    }

    return pieces;
}

PuzzlePiecePointer SimplePiecesFactory::createPiece(const QPoint &defaultPos) const
{
    QPointF sourceTL(splitterXs.at(defaultPos.x()    ), splitterYs.at(defaultPos.y()    ));
    QPointF sourceBR(splitterXs.at(defaultPos.x() + 1), splitterYs.at(defaultPos.y() + 1));

    return std::make_shared<PuzzlePiece>(defaultPos, sourcePixmap, QRectF(sourceTL, sourceBR));
}

} // Fifteen
