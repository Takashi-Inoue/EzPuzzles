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

#ifndef ABSTRACTMINEPIECE_H
#define ABSTRACTMINEPIECE_H

#include "IMinePiece.h"

namespace MineSweeper {

class AbstractMinePiece : public IMinePiece
{
public:
    AbstractMinePiece(const QRect &destRect)
        : m_destRect(destRect)
    {}

    void draw(QPainter &painter) override
    {
        if (!m_isChanged)
            return;

        painter.save();

        drawImpl(painter);

        painter.restore();

        m_isChanged = false;
    }

    void setEffect(EffectPointer effect) override
    {
        if (m_effect != effect)
            m_effect = effect;
    }

    void onTickFrame() override
    {
        if (m_effect != nullptr)
            m_isChanged |= m_effect->onTickFrame();
    }

protected:
    virtual void drawImpl(QPainter &painter) = 0;

    QRect m_destRect;
    EffectPointer m_effect;
    bool m_isChanged = true;
};

} // namespace MineSweeper

#endif // ABSTRACTMINEPIECE_H
