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
#include "EffectGraduallyImage.h"
#include <QDebug>

namespace Effect {

GraduallyImage::GraduallyImage(int waitFrames, int graduallyFrames, const QPixmap &pixmap, const QRectF &sourceRect) :
    AbstractEffect(waitFrames + graduallyFrames, false),
    waitFrames(waitFrames),
    graduallyFrames(graduallyFrames),
    pixmap(pixmap),
    sourceRect(sourceRect),
    frameCounter(waitFrames)
{
    Q_ASSERT(sourceRect.isValid());
}

bool GraduallyImage::onTickFrame()
{
    frameCounter = qMax(--frameCounter, 0);

    return (frameCounter == 0) & AbstractEffect::onTickFrame();
}

void GraduallyImage::draw(QPainter &painter, const QRectF &rect)
{
    if (frameCounter != 0)
        return;

    painter.save();
//    painter.setOpacity((nowFrame() - waitFrames) / graduallyFrames);
    painter.setOpacity(nowFrame() / totalFrames());

    painter.drawPixmap(rect, pixmap, sourceRect);

    painter.restore();
}

} // Effect
