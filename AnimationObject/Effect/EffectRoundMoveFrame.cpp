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
#include "EffectRoundMoveFrame.h"

#include <QDebug>

namespace Effect {

RoundMoveFrame::RoundMoveFrame(int width, const QColor &outer1, const QColor &inner1, const QColor &outer2, const QColor &inner2,
                               Qt::Edge startEdge, double startPoint, Direction direction, int totalFrameCount, bool isLoop) :
    AbstractEffect(totalFrameCount, isLoop),
    width(width),
    outerColor1(outer1),
    innerColor1(inner1),
    outerColor2(outer2),
    innerColor2(inner2),
    startEdge(startEdge),
    startPoint(startPoint),
    direction(direction)
{
    Q_ASSERT(width > 0);
    Q_ASSERT(startPoint >= 0 && startPoint <= 1);
}

void RoundMoveFrame::draw(QPainter &painter, const QRectF &rect)
{
    painter.save();
    painter.setClipRect(rect);
    painter.setPen(Qt::transparent);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    const auto rectInner = rect.marginsRemoved(QMarginsF(width, width, width, width));

    const double totalOuterLength = (rect.width() + rect.height()) * 2;
    const double totalInnerLength = (rectInner.width() + rectInner.height()) * 2;
    double nowOuterLength = totalOuterLength * nowFrame() / totalFrames();
    double nowInnerLength = totalInnerLength * nowFrame() / totalFrames();

    Qt::Edge currentEdge = startEdge;

    auto startOuterPos = pointOnEdge(rect, startEdge, startPoint);
    auto startInnerPos = pointOnEdge(rectInner, startEdge, startPoint);

    QList<Line> lines = {
        Line(startOuterPos, startInnerPos)
    };

    int divIndex = 0;

    for (int i = 0; i < 4; ++i) {
        auto outer = nextCorner(rect, currentEdge, direction);
        auto inner = nextCorner(rectInner, currentEdge, direction);

        if (nowOuterLength >= 0 && nowOuterLength < totalOuterLength) {
            double distanceToNextOuterCorner = calcDistance(startOuterPos, outer);
            double distanceToNextInnerCorner = calcDistance(startInnerPos, inner);

            if (distanceToNextOuterCorner > nowOuterLength) {
                QPointF divOuter = nextPoint(startOuterPos, currentEdge, direction, nowOuterLength);
                QPointF divInner = nextPoint(startInnerPos, currentEdge, direction, nowInnerLength);

                lines << Line(divOuter, divInner);

                divIndex = i + 1;

                nowOuterLength = -1;
            } else {
                nowOuterLength -= distanceToNextOuterCorner;
                nowInnerLength -= distanceToNextInnerCorner;
                startOuterPos = outer;
                startInnerPos = inner;
            }
        }

        lines << Line(outer, inner);

        currentEdge = nextEdge(currentEdge, direction);
    }

    if (lines.last() != lines.first())
        lines << lines.first();

    for (qsizetype i = 0, lim = lines.size() - 1; i < lim; ++i) {
        const auto &line1 = lines.at(i);
        const auto &line2 = lines.at(i + 1);

        const QColor &outerColor = ((divIndex == 0) | (i < divIndex)) ? outerColor1 : outerColor2;
        const QColor &innerColor = ((divIndex == 0) | (i < divIndex)) ? innerColor1 : innerColor2;

        drawPolygon(painter, rect, EdgeSquare(line1, line2), outerColor, innerColor);
    }

    painter.restore();
}

QPointF RoundMoveFrame::pointOnEdge(const QRectF &rect, Qt::Edge edge, double point) const
{
    if (edge == Qt::LeftEdge)
        return QPointF(rect.left(), rect.bottom() - rect.height() * point);

    if (edge == Qt::TopEdge)
        return QPointF(rect.left() + rect.width() * point, rect.top());

    if (edge == Qt::RightEdge)
        return QPointF(rect.right(), rect.top() + rect.height() * point);

    if (edge == Qt::BottomEdge)
        return QPointF(rect.right() - rect.width() * point, rect.bottom());

    Q_ASSERT_X(false, "RoundMoveFrame::pointOnEdge", "invalid edge");

    return rect.topLeft();
}

QPointF RoundMoveFrame::nextCorner(const QRectF &rect, Qt::Edge edge, Direction direction) const
{
    if (edge == Qt::LeftEdge)
        return direction == LeftHandTurn ? rect.bottomLeft() : rect.topLeft();

    if (edge == Qt::TopEdge)
        return direction == LeftHandTurn ? rect.topLeft() : rect.topRight();

    if (edge == Qt::RightEdge)
        return direction == LeftHandTurn ? rect.topRight() : rect.bottomRight();

    if (edge == Qt::BottomEdge)
        return direction == LeftHandTurn ? rect.bottomRight() : rect.bottomLeft();

    Q_ASSERT_X(false, "RoundMoveFrame::nextCorner", "invalid edge");

    return rect.topLeft();
}

QPointF RoundMoveFrame::nextPoint(const QPointF &point, Qt::Edge edge, RoundMoveFrame::Direction direction, double distance) const
{
    if (edge == Qt::LeftEdge)
        return direction == LeftHandTurn ? QPointF(point.x(), point.y() + distance) : QPointF(point.x(), point.y() - distance);

    if (edge == Qt::TopEdge)
        return direction == LeftHandTurn ? QPointF(point.x() - distance, point.y()) : QPointF(point.x() + distance, point.y());

    if (edge == Qt::RightEdge)
        return direction == LeftHandTurn ? QPointF(point.x(), point.y() - distance) : QPointF(point.x(), point.y() + distance);

    if (edge == Qt::BottomEdge)
        return direction == LeftHandTurn ? QPointF(point.x() + distance, point.y()) : QPointF(point.x() - distance, point.y());

    Q_ASSERT_X(false, "RoundMoveFrame::nextPoint", "invalid edge");

    return point;
}

Qt::Edge RoundMoveFrame::nextEdge(Qt::Edge edge, RoundMoveFrame::Direction direction) const
{
    if (edge == Qt::LeftEdge)
        return direction == LeftHandTurn ? Qt::BottomEdge : Qt::TopEdge;

    if (edge == Qt::TopEdge)
        return direction == LeftHandTurn ? Qt::LeftEdge : Qt::RightEdge;

    if (edge == Qt::RightEdge)
        return direction == LeftHandTurn ? Qt::TopEdge : Qt::BottomEdge;

    if (edge == Qt::BottomEdge)
        return direction == LeftHandTurn ? Qt::RightEdge : Qt::LeftEdge;

    Q_ASSERT_X(false, "RoundMoveFrame::nextEdge", "invalid edge");

    return edge;
}

double RoundMoveFrame::calcDistance(const QPointF &p1, const QPointF &p2) const
{
    Q_ASSERT(p1 != p2);
    Q_ASSERT((p1.x() == p2.x()) | (p1.y() == p2.y()));

    double result = (p1.x() == p2.x()) ? p1.y() - p2.y()
                                       : p1.x() - p2.x();

    return result >= 0 ? result : -result;
}

QLinearGradient RoundMoveFrame::createGradient(const QRectF &rect, Qt::Edge edge) const
{
    if (edge == Qt::LeftEdge)
        return QLinearGradient(rect.topLeft(), rect.topLeft() + QPoint(width, 0));

    if (edge == Qt::TopEdge)
        return QLinearGradient(rect.topLeft(), rect.topLeft() + QPoint(0, width));

    if (edge == Qt::RightEdge)
        return QLinearGradient(rect.topRight(), rect.topRight() - QPoint(width, 0));

    if (edge == Qt::BottomEdge)
        return QLinearGradient(rect.topRight(), rect.topRight() - QPoint(0, width));

    Q_ASSERT_X(false, "RoundMoveFrame::createGradient", "invalid edge");

    return QLinearGradient();
}

void RoundMoveFrame::drawPolygon(QPainter &painter, const QRectF &rect, const EdgeSquare &square,
                                 const QColor &outerColor, const QColor &innerColor) const
{
    auto gradient = createGradient(rect, square.edge());

    gradient.setColorAt(0, outerColor);
    gradient.setColorAt(1, innerColor);

    painter.setBrush(gradient);
    painter.drawPolygon(square.toPolygon());
}

} // Effect
