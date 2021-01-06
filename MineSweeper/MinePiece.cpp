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
#include "MinePiece.h"

#include "BlockPiece.h"

namespace MineSweeper {

MinePiece::MinePiece(const QRect &destRect)
    : AbstractMinePiece(destRect)
    , m_blockPiece(new BlockPiece(destRect.size()))
    , m_isOpened(false)
    , m_isLocked(false)
{
}

void MinePiece::open()
{
    if (m_isOpened | m_isLocked)
        return;

    auto blockPiece = new BlockPiece(m_destRect.size()
                                   , QColor("#E08080"), QColor("#C06060"), QColor("#600000"));

    m_blockPiece.reset(blockPiece);
    m_isOpened = true;
    m_isChanged = true;
}

void MinePiece::lock()
{
    if (m_isOpened)
        return;

    auto blockPiece = new BlockPiece(m_destRect.size()
                                   , QColor("#A0A0FF"), QColor("#8080E0"), QColor("#202080"));

    m_blockPiece.reset(blockPiece);
    m_isLocked = true;
    m_isChanged = true;
}

bool MinePiece::isOpen() const
{
    return m_isOpened;
}

bool MinePiece::isLock() const
{
    return m_isLocked;
}

bool MinePiece::isMine() const
{
    return true;
}

bool MinePiece::isNearMine() const
{
    return true;
}

bool MinePiece::isWall() const
{
    return false;
}

int MinePiece::countAroundMines() const
{
    return 0;
}

void MinePiece::drawImpl(QPainter &painter)
{
    painter.setOpacity(1.0);

    m_blockPiece->draw(painter, m_destRect);

    if (m_effect != nullptr)
        m_effect->draw(painter, m_destRect);
}

} // MineSweeper
