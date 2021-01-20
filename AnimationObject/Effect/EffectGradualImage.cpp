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
#include "EffectGradualImage.h"
#include <QDebug>

namespace Effect {

GradualImage::GradualImage(int graduallyFrames
                         , const QColor &background, const QPixmap &pixmap, const QRectF &sourceRect)
    : AbstractEffect(graduallyFrames, false)
    , m_graduallyFrames(graduallyFrames)
    , m_backgroundColor(background)
    , m_pixmap(pixmap)
    , m_sourceRect(sourceRect)
{
    Q_ASSERT(sourceRect.isValid());
}

bool GradualImage::onTickFrame()
{
    m_waitCounter = qMax(--m_waitCounter, 0);

    return (m_waitCounter == 0) & AbstractEffect::onTickFrame();
}

void GradualImage::draw(QPainter &painter, const QRectF &rect)
{
    if (m_waitCounter != 0)
        return;

    painter.save();

    painter.setOpacity(1.0);
    painter.fillRect(rect, m_backgroundColor);

    painter.setOpacity(nowFrame() / m_graduallyFrames);
    painter.drawPixmap(rect, m_pixmap, m_sourceRect);

    painter.restore();
}

} // Effect
