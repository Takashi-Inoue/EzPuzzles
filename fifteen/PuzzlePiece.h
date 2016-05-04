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
#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include "IPuzzlePiece.h"
#include <memory>

class ImageFragmentPiece;
class QPixmap;

namespace Fifteen {

class PuzzlePiece : public IPuzzlePiece
{
public:
    PuzzlePiece(const QPoint &pos, const QPixmap &sourceImage, const QRectF &sourceRect);
    ~PuzzlePiece() = default;

    // IPiece
    void draw(QPainter &painter, const QPointF &pos) override;
    void draw(QPainter &painter, const QPointF &pos, const QSizeF &targetSize) override;

    // IPuzzlePiece
    void setPos(const QPoint &position) override;
    void swapPos(IPuzzlePiece *) override;
    QPoint pos() const override;
    bool isPosCorrect() const override;

protected:
    std::unique_ptr<IPiece> imagePiece;
    QPoint position;
    const QPoint defaultPosition;
};

} // Fifteen

#endif // PUZZLEPIECE_H
