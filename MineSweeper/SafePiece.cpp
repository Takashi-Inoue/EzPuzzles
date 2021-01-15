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

#include "ImageFragmentPiece.h"
#include "NumberPieceFactory.h"

namespace MineSweeper {

SafePiece::SafePiece(int countAroundMines, const QRect &destRect, const QPixmap &pixmap
                   , const QRect &sourceRect)
    : AbstractMinePiece(destRect)
    , m_imagePiece(new ImageFragmentPiece(pixmap, sourceRect))
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

    double sub = qAbs(opacity - m_oldOpacity);

    if ((sub > 0.01) | (opacity == 1.0)) {
        m_oldOpacity = opacity;
        m_isChanged = true;
    }
}

bool SafePiece::isLocked() const
{
    return false;
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

void SafePiece::drawOpenedPiece(QPainter &painter) const
{
    fillRect(painter);
    painter.setOpacity(m_openOpacity);

    m_imagePiece->draw(painter, m_destRect);

    painter.setOpacity(1);

    if (m_numberPiece)
        m_numberPiece->draw(painter, m_destRect);

    if (m_effect)
        m_effect->draw(painter, m_destRect);
}

void SafePiece::fillRect(QPainter &painter) const
{
    painter.setOpacity(1);

    isNearMine() ? painter.fillRect(m_destRect, Qt::white)
                 : painter.fillRect(m_destRect, Qt::black);
}

} // MineSweeper
