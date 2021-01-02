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
#ifndef ROUNDMOVEFRAME_H
#define ROUNDMOVEFRAME_H

#include "AbstractEffect.h"

#include <QLinearGradient>
#include <QList>
#include <QPair>

namespace Effect {

class RoundMoveFrame : public AbstractEffect
{
public:
    enum Direction {
        LeftHandTurn,
        RightHandTurn,
    };

    RoundMoveFrame(int width, const QColor &outer1, const QColor &inner1, const QColor &outer2, const QColor &inner2,
                   Qt::Edge startEdge, double startPoint, Direction direction, int totalFrameCount, bool isLoop);

    // AbstractEffect
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    class Line
    {
    public:
        Line(const QPointF &outer, const QPointF &inner) :
            outer(outer),
            inner(inner)
        {
        }

        bool operator!=(const Line &other) const
        {
            return outer != other.outer && inner != other.inner;
        }

        QPointF outer;
        QPointF inner;
    };

    class EdgeSquare
    {
    public:
        EdgeSquare(const Line &line1, const Line &line2) :
            line1(line1),
            line2(line2)
        {
        }

        const QVector<QPointF> toPolygon() const
        {
            return {
                line1.outer,
                line2.outer,
                line2.inner,
                line1.inner,
            };
        }

        Qt::Edge edge() const
        {
            if (line1.outer.x() == line2.outer.x())
                return line1.inner.x() > line1.outer.x() ? Qt::LeftEdge : Qt::RightEdge;

            return line1.inner.y() > line1.outer.y() ? Qt::TopEdge : Qt::BottomEdge;
        }

        Line line1;
        Line line2;
    };

    QPointF pointOnEdge(const QRectF &rect, Qt::Edge edge, double point) const;
    QPointF nextCorner(const QRectF &rect, Qt::Edge edge, Direction direction) const;
    QPointF nextPoint(const QPointF &point, Qt::Edge edge, Direction direction, double distance) const;
    Qt::Edge nextEdge(Qt::Edge edge, Direction direction) const;
    double calcDistance(const QPointF &p1, const QPointF &p2) const;
    QLinearGradient createGradient(const QRectF &rect, Qt::Edge edge) const;
    void drawPolygon(QPainter &painter, const QRectF &rect, const EdgeSquare &square, const QColor &outerColor, const QColor &innerColor) const;

    int width;
    QColor outerColor1;
    QColor innerColor1;
    QColor outerColor2;
    QColor innerColor2;
    Qt::Edge startEdge;
    double startPoint;
    Direction direction;
};

} // Effect

#endif // ROUNDMOVEFRAME_H
