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

#ifndef SWAP_ENDINGBOARD_H
#define SWAP_ENDINGBOARD_H

#include "Fifteen/IBoard.h"

#include "BoardInfoPointer.h"
#include "Fifteen/FifteenPiecePointer.h"

#include <QElapsedTimer>
#include <QPixmap>

namespace Swap {

class EndingBoard : public Fifteen::IBoard
{
public:
    EndingBoard(BoardInfoPointer boardInfo, const QPixmap &sourceImage);

    void draw(QPainter &painter) override;
    bool onTickFrame() override;

protected:
    static constexpr int m_halfMSec = 750;

    QList<FifteenPiecePointer> m_pieces;
    QPixmap m_sourceImage;

    QElapsedTimer m_elapsedTimer;
};

} // namespace Swap

#endif // SWAP_ENDINGBOARD_H
