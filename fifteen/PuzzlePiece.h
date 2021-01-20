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

#include "IPuzzlePiece.h"

#include "BoardInformation.h"
#include "IPiece.h"
#include "Position.h"

class QPixmap;

namespace Fifteen {

class PuzzlePiece : public IPuzzlePiece
{
public:
    PuzzlePiece(BoardInfoPointer boardInfo, const QPoint &pieceDefaultPos, const QPixmap &sourceImage);

    // IPuzzlePiece
    bool onTickFrame() override;
    void skipAnimation() override;

    void draw(QPainter &painter) override;
    void setPos(const QPoint &pos) override;
    void setPosWithoutAnimation(const QPoint &pos) override;
    void setAnimation(AnimationPointer animation) override;
    void setEffect(EffectPointer effect) override;
    void setTransform(TransformPointer) override;
    AnimationPointer animation() const override;
    EffectPointer effect() const override;
    TransformPointer transform() const override;

    const Position &pos() const override;

protected:
    QScopedPointer<IPiece> m_imagePiece;
    BoardInfoPointer m_boardInfo;
    Position m_position;
    QRectF m_drawRect;

    AnimationPointer m_animation;
    EffectPointer m_effect;
    TransformPointer m_transform;

    bool m_isChanged = true;
};

} // Fifteen

#endif // PUZZLEPIECE_H
