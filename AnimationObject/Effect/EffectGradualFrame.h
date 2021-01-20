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
#ifndef EFFECT_GRADUALFRAME_H
#define EFFECT_GRADUALFRAME_H

#include "AbstractEffect.h"
#include <QPair>
#include <QPointF>
#include <QVector>

namespace Effect {

class GradualFrame : public AbstractEffect
{
public:
    GradualFrame(const QColor &outerStart, const QColor &outerEnd
                      , int totalFrameCount, bool isLoop);
    GradualFrame(int width, const QColor &outerStart, const QColor &innerStart
                      , const QColor &outerEnd, const QColor &innerEnd
                      , int totalFrameCount, bool isLoop);

    // AbstractEffect
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    virtual QColor currentColor(const QColor &start, const QColor &end) const;

    void drawSingleLineFrame(QPainter &painter, const QRectF &rect) const;
    void drawGradationFrame(QPainter &painter, const QRectF &rect) const;
    QList<QList<QPointF>> createEdgesPolygon(const QRectF &rect) const;
    QList<QPair<QPointF, QPointF>> createGradientsStartStop(const QRectF &rect) const;

    int m_width;
    QColor m_outerStartColor;
    QColor m_outerEndColor;
    QColor m_innerStartColor;
    QColor m_innerEndColor;
};

} // Effect

#endif // EFFECT_GRADUALFRAME_H
