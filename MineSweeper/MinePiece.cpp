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

#include <QDebug>

namespace MineSweeper {

MinePiece::MinePiece(const QRect &destRect)
    : AbstractMinePiece(destRect)
    , m_openedPiece(BlockPiece::create(destRect.size(), BlockPiece::Colors::red))
    , m_isLocked(false)
{
}

void MinePiece::open()
{
    if (m_isLocked)
        return;

    AbstractMinePiece::open();
}

void MinePiece::lock()
{
    if (m_isLocked)
        return;

    m_closedPiece = BlockPiece::create(m_destRect.size(), BlockPiece::Colors::blue);
    m_isLocked = true;
    m_isOpened = false;
    m_isChanged = true;
}

void MinePiece::press()
{
    if (!m_isLocked)
        AbstractMinePiece::press();
}

void MinePiece::release()
{
    if (!m_isLocked)
        AbstractMinePiece::release();
}

bool MinePiece::isLocked() const
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

void MinePiece::drawOpenedPiece(QPainter &painter) const
{
    painter.setOpacity(1.0);

    m_openedPiece->draw(painter, m_destRect);

    if (m_effect != nullptr)
        m_effect->draw(painter, m_destRect);
}

} // MineSweeper
