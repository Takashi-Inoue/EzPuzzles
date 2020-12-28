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
#ifndef GRADUALLYBLINKFRAME_H
#define GRADUALLYBLINKFRAME_H

#include "AbstractEffect.h"
#include <QPair>
#include <QPointF>
#include <QVector>

namespace Effect {

class GraduallyBlinkFrame : public AbstractEffect
{
public:
    GraduallyBlinkFrame(const QColor &outerStart, const QColor &outerEnd, int totalFrameCount, bool isLoop);
    GraduallyBlinkFrame(int width, const QColor &outerStart, const QColor &innerStart, const QColor &outerEnd, const QColor &innerEnd, int totalFrameCount, bool isLoop);

    // AbstractEffect
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    typedef QPair<QPointF, QPointF> PointPair;

    void drawSingleLineFrame(QPainter &painter, const QRectF &rect) const;
    void drawGradationFrame(QPainter &painter, const QRectF &rect) const;
    QColor currentColor(const QColor &start, const QColor &end) const;
    QVector<QVector<QPointF>> createEdgesPolygon(const QRectF &rect) const;
    QVector<PointPair> createGradientsStartStop(const QRectF &rect) const;

    int width;
    QColor outerStartColor;
    QColor outerEndColor;
    QColor innerStartColor;
    QColor innerEndColor;
};

} // Effect

#endif // GRADUALLYBLINKFRAME_H
