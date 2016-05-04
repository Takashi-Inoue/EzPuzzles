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
#include "ImagePiece.h"

ImagePiece::ImagePiece(const QPixmap &pixmap) :
    pixmap(pixmap)
{
    Q_ASSERT(!pixmap.isNull());
}

void ImagePiece::draw(QPainter &painter, const QPointF &pos)
{
    draw(painter, pos, pixmap.size());
}

void ImagePiece::draw(QPainter &painter, const QPointF &pos, const QSizeF &targetSize)
{
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.drawPixmap(QRectF(pos, targetSize), pixmap, pixmap.rect());
}
