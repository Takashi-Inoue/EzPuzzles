/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
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

    typedef QPair<QPointF, QPointF> Points;

    QList<Points> pointsList = {
        Points(startOuterPos, startInnerPos)
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

                pointsList << Points(divOuter, divInner);

                divIndex = i + 1;

                nowOuterLength = -1;
            } else {
                nowOuterLength -= distanceToNextOuterCorner;
                nowInnerLength -= distanceToNextInnerCorner;
                startOuterPos = outer;
                startInnerPos = inner;
            }
        }

        pointsList << Points(outer, inner);

        currentEdge = nextEdge(currentEdge, direction);
    }

    if (pointsList.last() != pointsList.first())
        pointsList << pointsList.first();

    for (int i = 0, lim = pointsList.size() - 1; i < lim; ++i) {
        const auto &points1 = pointsList.at(i);
        const auto &points2 = pointsList.at(i + 1);

        auto gradient = createGradient(rect, edgeFromPolygon(points1.first, points1.second, points2.first));
        gradient.setColorAt(0, i < divIndex ? outerColor1 : outerColor2);
        gradient.setColorAt(1, i < divIndex ? innerColor1 : innerColor2);

        QVector<QPointF> polygon = {
            points1.first,
            points1.second,
            points2.second,
            points2.first,
        };

        painter.setBrush(gradient);
        painter.drawPolygon(polygon);
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

Qt::Edge RoundMoveFrame::edgeFromPolygon(const QPointF &outer1, const QPointF &inner1, const QPointF &outer2) const
{
    if (outer1.x() == outer2.x())
        return inner1.x() > outer1.x() ? Qt::LeftEdge : Qt::RightEdge;

    return inner1.y() > outer1.y() ? Qt::TopEdge : Qt::BottomEdge;
}

double RoundMoveFrame::calcDistance(const QPointF &p1, const QPointF &p2) const
{
    Q_ASSERT(p1 != p2);
    Q_ASSERT((p1.x() == p2.x()) | (p1.y() == p2.y()));

    double result = (p1.x() == p2.x()) ? p1.y() - p2.y()
                                       : p1.x() - p2.x();

    return result >= 0 ? result : -result;
}

QLinearGradient RoundMoveFrame::createGradient(const QRectF &rect, Qt::Edge edge)
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

} // Effect
