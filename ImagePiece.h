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
#ifndef IMAGEPIECE_H
#define IMAGEPIECE_H

#include "IPiece.h"
#include <QPixmap>

class ImagePiece : public IPiece
{
public:
    ImagePiece(const QPixmap &pixmap);

    void draw(QPainter &painter, const QPointF &pos) override;
    void draw(QPainter &painter, const QPointF &pos, const QSizeF &targetSize) override;

protected:
    QPixmap pixmap;
};

#endif // IMAGEPIECE_H
