/*
 * Copyright 2021 Takashi Inoue
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

#include "AbstractMinePiece.h"

#include "BlockPiece.h"

namespace MineSweeper {

AbstractMinePiece::AbstractMinePiece(const QRect &destRect)
    : m_closedPiece(BlockPiece::create(destRect.size(), BlockPiece::Colors::gray))
    , m_destRect(destRect)
{
}

void AbstractMinePiece::draw(QPainter &painter)
{
    if (!m_isChanged)
        return;

    painter.save();

    m_isOpened ? drawOpenedPiece(painter)
               : drawClosedPiece(painter);

    painter.restore();

    m_isChanged = false;
}

void AbstractMinePiece::setEffect(EffectPointer effect)
{
    if (m_effect != effect)
        m_effect = effect;
}

void AbstractMinePiece::onTickFrame()
{
    if (m_effect != nullptr)
        m_isChanged |= m_effect->onTickFrame();
}

bool AbstractMinePiece::isOpened() const
{
    return m_isOpened;
}

void AbstractMinePiece::open()
{
    if (m_isOpened)
        return;

    m_isOpened = true;
    m_isChanged = true;
}

void AbstractMinePiece::press()
{
    if (m_isOpened)
        return;

    m_closedPiece = BlockPiece::create(m_destRect.size(), BlockPiece::Colors::grayPressed);
    m_isChanged = true;
}

void AbstractMinePiece::release()
{
    if (m_isOpened)
        return;

    m_closedPiece = BlockPiece::create(m_destRect.size(), BlockPiece::Colors::gray);
    m_isChanged = true;
}

void AbstractMinePiece::drawClosedPiece(QPainter &painter) const
{
    painter.setOpacity(1.0);

    m_closedPiece->draw(painter, m_destRect);

    if (m_effect != nullptr)
        m_effect->draw(painter, m_destRect);
}

} // namespace MineSweeper
