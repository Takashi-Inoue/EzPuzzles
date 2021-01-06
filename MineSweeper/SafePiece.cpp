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
#include "SafePiece.h"

#include "SwitchImagePiece.h"
#include "NumberPieceFactory.h"

namespace MineSweeper {

SafePiece::SafePiece(int countAroundMines, const QRect &destRect, const QPixmap &pixmap
                   , const QRect &sourceRect)
    : AbstractMinePiece(destRect)
    , m_switchImagePiece(new SwitchImagePiece(pixmap, sourceRect))
    , m_countAroundMines(countAroundMines)
{
    Q_ASSERT(countAroundMines >= 0 && countAroundMines < 9);

    if (isNearMine()) {
        m_openOpacity = 0.5;
        m_numberPiece = NumberPieceFactory::getPiece(countAroundMines, m_destRect.size());
    }
}

void SafePiece::setOpenPieceOpacity(double opacity)
{
    if (isNearMine())
        return;

    m_openOpacity = opacity;

    double div = opacity - m_oldOpacity;

    div = (div >= 0) ? div : -div;

    if ((div > 0.01) | (opacity == 1.0)) {
        m_oldOpacity = opacity;
        m_isChanged = true;
    }
}

void SafePiece::open()
{
    if (m_switchImagePiece->isOpen())
        return;

    m_switchImagePiece->open();

    m_isChanged = true;
}

void SafePiece::lock()
{
    if (m_switchImagePiece->isLock())
        return;

    m_switchImagePiece->lock();

    m_isChanged = true;
}

bool SafePiece::isOpen() const
{
    return m_switchImagePiece->isOpen();
}

bool SafePiece::isLock() const
{
    return m_switchImagePiece->isLock();
}

bool SafePiece::isMine() const
{
    return false;
}

bool SafePiece::isNearMine() const
{
    return m_countAroundMines > 0;
}

bool SafePiece::isWall() const
{
    return false;
}

int SafePiece::countAroundMines() const
{
    return m_countAroundMines;
}

void SafePiece::drawImpl(QPainter &painter)
{
    if (isOpen()) {
        fillRect(painter);
        painter.setOpacity(m_openOpacity);
    }

    m_switchImagePiece->draw(painter, m_destRect);

    painter.setOpacity(1);

    if (m_numberPiece != nullptr && isOpen())
        m_numberPiece->draw(painter, m_destRect);

    if (m_effect != nullptr)
        m_effect->draw(painter, m_destRect);
}

void SafePiece::fillRect(QPainter &painter)
{
    painter.setOpacity(1);

    isNearMine() ? painter.fillRect(m_destRect, Qt::white)
                 : painter.fillRect(m_destRect, Qt::black);
}

} // MineSweeper
