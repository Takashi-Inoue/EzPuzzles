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

#include <QDebug>

namespace Fifteen {

PuzzlePiece::PuzzlePiece(BoardInfoPointer boardInfo, const QPoint &pieceDefaultPos
                       , const QPixmap &sourceImage)
    : m_imagePiece(new ImageFragmentPiece(sourceImage, boardInfo->rectFromPiecePos(pieceDefaultPos)))
    , m_boardInfo(boardInfo)
    , m_position(pieceDefaultPos)
{
}

bool PuzzlePiece::onTickFrame()
{
    if (m_animation)
        m_isChanged |= m_animation->onTickFrame();

    if (m_effect)
        m_isChanged |= m_effect->onTickFrame();

    if (m_transform)
        m_isChanged |= m_transform->onTickFrame();

    return m_isChanged;
}

void PuzzlePiece::skipAnimation()
{
    m_isChanged = true;

    if (m_animation) {
        m_animation->skipAnimation();
        m_drawRect = m_animation->rect();
    }

    if (m_effect)
        m_effect->skipAnimation();

    if (m_transform)
        m_transform->skipAnimation();
}

void PuzzlePiece::draw(QPainter &painter)
{
    if (!m_isChanged)
        return;

    painter.save();

    if (m_animation) {
        auto rect = m_animation->rect();

        if (!rect.isNull())
            m_drawRect = rect;
    }

    if (m_drawRect.isNull())
        m_drawRect = m_boardInfo->rectFromPiecePos(m_position.currentPos());

    QRectF rect = m_drawRect;

    if (m_transform) {
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
        painter.fillRect(m_drawRect, Qt::black);
        rect = m_transform->mapInRect(m_drawRect);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }

    m_imagePiece->draw(painter, rect);

    if (m_effect)
        m_effect->draw(painter, rect);

    painter.restore();

    m_isChanged = false;
}

void PuzzlePiece::setPos(const QPoint &pos)
{
    if (m_animation == nullptr) {
        setPosWithoutAnimation(pos);

        return;
    }

    m_position.setPos(pos);

    m_animation->start(m_drawRect, m_boardInfo->rectFromPiecePos(pos));

    if (m_transform != nullptr)
        m_transform->start(m_drawRect.size());

    m_isChanged = true;
}

void PuzzlePiece::setPosWithoutAnimation(const QPoint &pos)
{
    m_position.setPos(pos);

    m_drawRect = m_boardInfo->rectFromPiecePos(pos);

    m_isChanged = true;
}

void PuzzlePiece::setAnimation(AnimationPointer animation)
{
    if (m_animation != animation)
        m_animation = animation;
}

void PuzzlePiece::setEffect(EffectPointer effect)
{
    if (m_effect != effect)
        m_effect = effect;
}

void PuzzlePiece::setTransform(TransformPointer transform)
{
    if (m_transform != transform)
        m_transform = transform;
}

const Position &PuzzlePiece::pos() const
{
    return m_position;
}

AnimationPointer PuzzlePiece::animation() const
{
    return m_animation;
}

EffectPointer PuzzlePiece::effect() const
{
    return m_effect;
}

TransformPointer PuzzlePiece::transform() const
{
    return m_transform;
}

} // Fifteen
