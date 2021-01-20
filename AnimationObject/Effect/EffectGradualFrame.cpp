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
#include "EffectGradualFrame.h"

#include <QDebug>

namespace Effect {

GradualFrame::GradualFrame(const QColor &outerStart, const QColor &outerEnd
                                       , int totalFrameCount, bool isLoop)
    : GradualFrame(1, outerStart, outerStart, outerEnd, outerEnd, totalFrameCount, isLoop)
{
}

GradualFrame::GradualFrame(int width, const QColor &outerStart, const QColor &innerStart
                                       , const QColor &outerEnd, const QColor &innerEnd
                                       , int totalFrameCount, bool isLoop)
    : AbstractEffect(totalFrameCount, isLoop)
    , m_width(width)
    , m_outerStartColor(outerStart)
    , m_outerEndColor(outerEnd)
    , m_innerStartColor(innerStart)
    , m_innerEndColor(innerEnd)
{
    Q_ASSERT(width > 0);
}

void GradualFrame::draw(QPainter &painter, const QRectF &rect)
{
    m_width == 1 ? drawSingleLineFrame(painter, rect)
                 : drawGradationFrame(painter, rect);
}

QColor GradualFrame::currentColor(const QColor &start, const QColor &end) const
{
    if (totalFrames() == 0.0)
        return start;

    float pct = float(nowFrame() / totalFrames());

    QColor color;

    color.setRedF  ((end.redF()   - start.redF())   * pct + start.redF());
    color.setGreenF((end.greenF() - start.greenF()) * pct + start.greenF());
    color.setBlueF ((end.blueF()  - start.blueF())  * pct + start.blueF());
    color.setAlphaF((end.alphaF() - start.alphaF()) * pct + start.alphaF());

    return color;
}

void GradualFrame::drawSingleLineFrame(QPainter &painter, const QRectF &rect) const
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing, false);
    painter.setClipRect(rect);

    QColor color = currentColor(m_outerStartColor, m_outerEndColor);

    painter.setPen(QPen(color, 1.0));
    painter.drawRect(rect.marginsRemoved(QMarginsF(0, 0, 1, 1)));

    painter.restore();
}

void GradualFrame::drawGradationFrame(QPainter &painter, const QRectF &rect) const
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing);
    painter.setClipRect(rect);
    painter.setPen(Qt::transparent);

    QLinearGradient gradient(rect.topLeft(), rect.topLeft() + QPointF(m_width, 0));

    QColor outer = currentColor(m_outerStartColor, m_outerEndColor);
    QColor inner = currentColor(m_innerStartColor, m_innerEndColor);

    gradient.setColorAt(0, outer);
    gradient.setColorAt(1, inner);

    // left - top - right - bottom
    QList<QList<QPointF>> edgesPolygon = createEdgesPolygon(rect);
    QList<QPair<QPointF, QPointF>> gradientsStartStop = createGradientsStartStop(rect);

    for (int i = 0; i < 4; ++i) {
        const QPair<QPointF, QPointF> &startStop = gradientsStartStop.at(i);

        gradient.setStart(startStop.first);
        gradient.setFinalStop(startStop.second);

        painter.setBrush(gradient);
        painter.drawPolygon(edgesPolygon.at(i));
    }

    painter.restore();
}

QList<QList<QPointF>> GradualFrame::createEdgesPolygon(const QRectF &rect) const
{
    // left - top - right - bottom
    return {
        {
            rect.topLeft(),
            rect.topLeft()    + QPointF(m_width,  m_width),
            rect.bottomLeft() + QPointF(m_width, -m_width),
            rect.bottomLeft(),
        },
        {
            rect.topLeft(),
            rect.topLeft()  + QPointF( m_width, m_width),
            rect.topRight() + QPointF(-m_width, m_width),
            rect.topRight(),
        },
        {
            rect.topRight(),
            rect.topRight()    + QPointF(-m_width,  m_width),
            rect.bottomRight() + QPointF(-m_width, -m_width),
            rect.bottomRight(),
        },
        {
            rect.bottomRight(),
            rect.bottomRight() + QPointF(-m_width, -m_width),
            rect.bottomLeft()  + QPointF( m_width, -m_width),
            rect.bottomLeft(),
        },
    };
}

QList<QPair<QPointF, QPointF>> GradualFrame::createGradientsStartStop(const QRectF &rect) const
{
    // left - top - right - bottom
    return {
        qMakePair(rect.topLeft(),     rect.topLeft()     + QPointF(m_width, 0)),
        qMakePair(rect.topLeft(),     rect.topLeft()     + QPointF(0, m_width)),
        qMakePair(rect.bottomRight(), rect.bottomRight() + QPointF(-m_width, 0)),
        qMakePair(rect.bottomRight(), rect.bottomRight() + QPointF(0, -m_width)),
    };
}

} // Effect
