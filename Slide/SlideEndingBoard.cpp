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

#include "SlideEndingBoard.h"

#include "AnimationFactory.h"
#include "AnimationObject/Effect/CompositeEffect.h"
#include "Fifteen/FifteenFactory.h"

namespace Slide {

EndingBoard::EndingBoard(BoardInfoPointer boardInfo, const QPixmap &sourceImage
                       , const QPoint &blankPos)
    : m_pieces(Fifteen::Factory().createPieces(boardInfo, sourceImage))
    , m_sourceImage(sourceImage)
{
    static constexpr uchar graduallyFrames = 60;

    for (FifteenPiecePointer &piece : m_pieces)
        piece->setEffect(AnimationFactory::fadeFrame(graduallyFrames * 2));

    FifteenPiecePointer &blankPiece = m_pieces[blankPos.y() * boardInfo->xCount() + blankPos.x()];

    const QRectF rect = boardInfo->rectFromPiecePos(blankPiece->pos().defaultPos());

    EffectPointer graduallyImage = AnimationFactory::graduallyImage(graduallyFrames
                                                                  , Qt::black, m_sourceImage, rect);

    auto effects = QSharedPointer<Effect::CompositeEffect>::create();

    effects->addEffect(graduallyImage);
    effects->addEffect(blankPiece->effect());

    blankPiece->setEffect(effects);
}

void EndingBoard::draw(QPainter &painter)
{
    for (FifteenPiecePointer &piece : m_pieces)
        piece->draw(painter);
}

bool EndingBoard::onTickFrame()
{
    bool isChanged = false;

    for (FifteenPiecePointer &piece : m_pieces)
        isChanged |= piece->onTickFrame();

    return isChanged;
}

} // namespace Slide
