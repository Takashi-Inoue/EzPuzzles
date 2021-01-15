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
#include "IPiece.h"

namespace MineSweeper {

class AbstractMinePiece : public IMinePiece
{
public:
    AbstractMinePiece(const QRect &destRect);

    void draw(QPainter &painter) override;
    void setEffect(EffectPointer effect) override;
    void onTickFrame() override;

    bool isOpened() const override;

    void open() override;
    void press() override;
    void release() override;

protected:
    virtual void drawClosedPiece(QPainter &painter) const;
    virtual void drawOpenedPiece(QPainter &painter) const = 0;

    QSharedPointer<IPiece> m_closedPiece;
    QRect m_destRect;
    EffectPointer m_effect;

    bool m_isOpened = false;
    bool m_isChanged = true;
};

} // namespace MineSweeper

#endif // ABSTRACTMINEPIECE_H
