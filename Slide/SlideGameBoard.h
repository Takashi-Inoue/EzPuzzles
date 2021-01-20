/*
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

#ifndef SLIDE_GAMEBOARD_H
#define SLIDE_GAMEBOARD_H

#include "Fifteen/AbstractGameBoard.h"

namespace Slide {

class GameBoard : public Fifteen::AbstractGameBoard
{
public:
    GameBoard(BoardInfoPointer boardInformation, QPoint &currentBlankPos);

    void shufflePieces() override;
    void movePiece(const QPoint &from, const QPoint &to) override;
    int frameCountToMove() const override;
    bool isFinishedMoving() const override;

    void initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions = {}) override;

protected:
    QPoint &m_currentBlankPos;
};

} // namespace Slide

#endif // SLIDE_GAMEBOARD_H
