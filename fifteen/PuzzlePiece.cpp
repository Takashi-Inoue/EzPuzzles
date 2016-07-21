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
#include "PuzzlePiece.h"
#include "ImageFragmentPiece.h"
#include "SourceImage.h"

namespace Fifteen {

PuzzlePiece::PuzzlePiece(BoardInfoPointer boardInfo, const QPoint &pieceDefaultPos, const QPixmap &sourceImage) :
    imagePiece(std::make_unique<ImageFragmentPiece>(sourceImage, boardInfo->rectFromPiecePos(pieceDefaultPos))),
    boardInfo(boardInfo),
    position(pieceDefaultPos)
{
}

void PuzzlePiece::draw(QPainter &painter)
{
    if (animObj != nullptr) {
        auto rect = animObj->rect();

        if (!rect.isNull())
            drawRect = rect;
    }

    imagePiece->draw(painter, drawRect);

    if (effectObj != nullptr)
        effectObj->draw(painter, drawRect);
}

void PuzzlePiece::setPos(const QPoint &pos)
{
    if (animObj == nullptr) {
        setPosWithoutAnimation(pos);

        return;
    }

    position.setPos(pos);

    animObj->start(drawRect, boardInfo->rectFromPiecePos(pos));
}

void PuzzlePiece::setPosWithoutAnimation(const QPoint &pos)
{
    position.setPos(pos);

    drawRect = boardInfo->rectFromPiecePos(pos);
}

void PuzzlePiece::setAnimation(AnimationPointer animation)
{
    animObj = animation;
}

void PuzzlePiece::setEffect(EffectPointer effect)
{
    effectObj = effect;
}

const Position &PuzzlePiece::pos() const
{
    return position;
}

void PuzzlePiece::onTickFrame()
{
    if (animObj != nullptr)
        animObj->onTickFrame();

    if (effectObj != nullptr)
        effectObj->onTickFrame();
}

void PuzzlePiece::skipAnimation()
{
    if (animObj != nullptr) {
        animObj->skipAnimation();
        drawRect = animObj->rect();
    }

    if (effectObj != nullptr)
        effectObj->skipAnimation();
}

bool PuzzlePiece::isLoopAnimation()
{
    return false;
}

const AnimationPointer &PuzzlePiece::animation() const
{
    return animObj;
}

const EffectPointer &PuzzlePiece::effect() const
{
    return effectObj;
}

bool PuzzlePiece::isFinishedAnimation()
{
    return false;
}

} // Fifteen
