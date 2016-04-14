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
#include "ImageFragmentPiece.h"

ImageFragmentPiece::ImageFragmentPiece(const QPixmap &sourcePixmap, const QRect &sourceRect) :
    sourcePixmap(sourcePixmap),
    sourceRect(sourceRect)
{
    Q_ASSERT(!sourcePixmap.isNull());
    Q_ASSERT(sourceRect.isValid());
}

void ImageFragmentPiece::draw(QPainter &painter, const QPoint &pos)
{
    draw(painter, pos, sourceRect.size());
}

void ImageFragmentPiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.drawPixmap(QRect(pos, targetSize), sourcePixmap, sourceRect);
}
