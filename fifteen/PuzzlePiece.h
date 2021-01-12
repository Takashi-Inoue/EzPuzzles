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
#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include "IPiece.h"
#include "IPuzzlePiece.h"
#include "BoardInformation.h"
#include "Position.h"
#include <memory>

class ImageFragmentPiece;
class QPixmap;

namespace Fifteen {

class PuzzlePiece : public IPuzzlePiece
{
public:
    PuzzlePiece(BoardInfoPointer boardInfo, const QPoint &pieceDefaultPos, const QPixmap &sourceImage);

    // IPuzzlePiece
    void onTickFrame() override;
    void skipAnimation() override;

    void draw(QPainter &painter) override;
    void setPos(const QPoint &pos) override;
    void setPosWithoutAnimation(const QPoint &pos) override;
    void setAnimation(AnimationPointer animation) override;
    void setEffect(EffectPointer effect) override;
    void setTransform(TransformPointer) override;
    const AnimationPointer &animation() const override;
    const EffectPointer &effect() const override;

    const Position &pos() const override;

protected:
    std::unique_ptr<IPiece> imagePiece;
    BoardInfoPointer boardInfo;
    Position position;
    QRectF drawRect;

    AnimationPointer animObj;
    EffectPointer effectObj;
    TransformPointer transformObj;

    bool isChanged;
};

} // Fifteen

#endif // PUZZLEPIECE_H
