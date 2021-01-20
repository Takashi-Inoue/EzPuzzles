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

#ifndef SLIDE_ENDINGBOARD_H
#define SLIDE_ENDINGBOARD_H

#include "BoardInfoPointer.h"
#include "Fifteen/IBoard.h"
#include "Fifteen/FifteenPiecePointer.h"

#include <QPixmap>

namespace Slide {

class EndingBoard : public Fifteen::IBoard
{
public:
    EndingBoard(BoardInfoPointer boardInfo, const QPixmap &sourceImage, const QPoint &blankPos);

    void draw(QPainter &painter) override;
    bool onTickFrame() override;

protected:
    QList<FifteenPiecePointer> m_pieces;
    QPixmap m_sourceImage;
};

} // namespace Slide

#endif // SLIDE_ENDINGBOARD_H
