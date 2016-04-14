﻿/*
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
#include "SwitchImagePiece.h"

#include "BlockPiece.h"
#include "ImagePiece.h"
#include "ImageFragmentPiece.h"

SwitchImagePiece::SwitchImagePiece(const QPixmap &pixmap, const QRect sourceRect) :
    blockPiece(std::make_unique<BlockPiece>(pixmap.size())),
    imagePiece(nullptr),
    isOpened(false),
    isLocked(false),
    openOpacity(1.0)
{
    if (sourceRect.isNull()) {
        blockPiece = std::make_unique<BlockPiece>(pixmap.size());
        imagePiece = std::make_unique<ImagePiece>(pixmap);
    } else {
        blockPiece = std::make_unique<BlockPiece>(sourceRect.size());
        imagePiece = std::make_unique<ImageFragmentPiece>(pixmap, sourceRect);
    }
}

void SwitchImagePiece::draw(QPainter &painter, const QPoint &pos)
{
    if (isOpen()) {
        painter.setOpacity(openOpacity);

        imagePiece->draw(painter, pos);
    } else {
        painter.setOpacity(1.0);

        blockPiece->draw(painter, pos);
    }
}

void SwitchImagePiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    if (isOpen()) {
        painter.setOpacity(openOpacity);

        imagePiece->draw(painter, pos, targetSize);
    } else {
        painter.setOpacity(1.0);

        blockPiece->draw(painter, pos, targetSize);
    }
}

void SwitchImagePiece::open()
{
    isOpened = true;
}

void SwitchImagePiece::close()
{
    isOpened = false;
}

void SwitchImagePiece::lock()
{
    isLocked = true;
}

bool SwitchImagePiece::isOpen() const
{
    return isOpened;
}

bool SwitchImagePiece::isLock() const
{
    return isLocked;
}

void SwitchImagePiece::setOpenPieceOpacity(double opacity)
{
    Q_ASSERT(opacity >= 0 && opacity <= 1.0);

    openOpacity = opacity;
}
