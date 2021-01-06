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
#ifndef SAFEPIECE_H
#define SAFEPIECE_H

#include "AbstractMinePiece.h"

namespace MineSweeper {

class SafePiece : public AbstractMinePiece
{
public:
    SafePiece(int countAroundMines, const QRect &destRect, const QPixmap &pixmap
            , const QRect &sourceRect);

    // IMinePiece
    void setOpenPieceOpacity(double opacity) override;

    void open() override;
    void lock() override;

    bool isOpen() const override;
    bool isLock() const override;
    bool isMine() const override;
    bool isNearMine() const override;
    bool isWall() const override;

    int countAroundMines() const override;

protected:
    // AbstractMinePiece
    void drawImpl(QPainter &painter) override;

    void fillRect(QPainter &painter);

    QScopedPointer<ISwitchPiece> m_switchImagePiece;
    QSharedPointer<IPiece> m_numberPiece;

    int m_countAroundMines;
    double m_openOpacity = 1.0;
    double m_oldOpacity = 1.0;
};

} // MineSweeper

#endif // SAFEPIECE_H
