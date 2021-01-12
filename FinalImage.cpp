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
#include "FinalImage.h"

FinalImage::FinalImage(QPixmap pixmap, QObject *parent)
    : QObject(parent)
    , m_pixmap(pixmap)
{
    Q_ASSERT(!pixmap.isNull());
}

void FinalImage::draw(QPainter &painter)
{
    drawFinalImage(painter);
}

QRect FinalImage::drawRect(QPainter &painter) const
{
    QSize destSize = m_pixmap.size().scaled(painter.viewport().size(), Qt::KeepAspectRatio);
    QRect destRect({0, 0}, destSize);

    destRect.moveCenter(painter.viewport().center());

    return destRect;
}

QRect FinalImage::drawFinalImage(QPainter &painter) const
{
    QRect destRect = drawRect(painter);

    painter.drawPixmap(destRect, m_pixmap, m_pixmap.rect());

    return destRect;
}
