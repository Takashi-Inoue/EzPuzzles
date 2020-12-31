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
#include "SwitchImagePiece.h"

#include "BlockPiece.h"
#include "ImagePiece.h"
#include "ImageFragmentPiece.h"

SwitchImagePiece::SwitchImagePiece(const QPixmap &pixmap, const QRect sourceRect)
{
    if (sourceRect.isNull()) {
        m_blockPiece.reset(new BlockPiece(pixmap.size()));
        m_imagePiece.reset(new ImagePiece(pixmap));
    } else {
        m_blockPiece.reset(new BlockPiece(sourceRect.size()));
        m_imagePiece.reset(new ImageFragmentPiece(pixmap, sourceRect));
    }
}

void SwitchImagePiece::draw(QPainter &painter, const QPointF &pos)
{
    isOpen() ? m_imagePiece->draw(painter, pos)
             : m_blockPiece->draw(painter, pos);
}

void SwitchImagePiece::draw(QPainter &painter, const QRectF &rect)
{
    isOpen() ? m_imagePiece->draw(painter, rect)
             : m_blockPiece->draw(painter, rect);
}

void SwitchImagePiece::open()
{
    m_isOpened = true;
}

void SwitchImagePiece::close()
{
    m_isOpened = false;
}

void SwitchImagePiece::lock()
{
    m_isLocked = true;
}

bool SwitchImagePiece::isOpen() const
{
    return m_isOpened;
}

bool SwitchImagePiece::isLock() const
{
    return m_isLocked;
}
