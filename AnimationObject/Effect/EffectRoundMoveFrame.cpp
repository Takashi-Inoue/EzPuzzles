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

    auto rectInner = rect.marginsRemoved(QMarginsF(width, width, width, width));

    double totalOuterLength = (rect.width() + rect.height()) * 2;
    double totalInnerLength = (rectInner.width() + rectInner.height()) * 2;
    double nowOuterLength = totalOuterLength * nowFrame() / totalFrames();
    double nowInnerLength = totalInnerLength * nowFrame() / totalFrames();

//    qDebug() << totalOuterLength << nowFrame() << totalFrames() << nowOuterLength;

    bool isColorChanged = false;

    Qt::Edge currentEdge = startEdge;

    auto startOuterPos = pointOnEdge(rect, startEdge, startPoint);
    auto startInnerPos = pointOnEdge(rectInner, startEdge, startPoint);

    for (int i = 0; i < 5; ++i) {
        auto nextOuterCorner = nextCorner(rect, currentEdge, direction);
        auto nextInnerCorner = nextCorner(rectInner, currentEdge, direction);

        double distOuter = calcDistance(startOuterPos, nextOuterCorner);
        double distInner = calcDistance(startInnerPos, nextInnerCorner);

        auto gradient = createGradient(rect, currentEdge);
        gradient.setColorAt(0, isColorChanged ? outerColor2 : outerColor1);
        gradient.setColorAt(1, isColorChanged ? innerColor2 : innerColor1);

        QVector<QPointF> polygon;

        if (!isColorChanged && distOuter > nowOuterLength) {
            auto nextOuterPos = nextPoint(startOuterPos, currentEdge, direction, nowOuterLength);
            auto nextInnerPos = nextPoint(startInnerPos, currentEdge, direction, nowInnerLength);

            polygon << startOuterPos << startInnerPos << nextInnerPos << nextOuterPos;

            startOuterPos = nextOuterPos;
            startInnerPos = nextInnerPos;

            isColorChanged = true;
        } else {
            polygon << startOuterPos << startInnerPos << nextInnerCorner << nextOuterCorner;

            startOuterPos = nextOuterCorner;
            startInnerPos = nextInnerCorner;

            currentEdge = nextEdge(currentEdge, direction);

            nowOuterLength -= distOuter;
            nowInnerLength -= distInner;
        }

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
    Q_ASSERT((direction == LeftHandTurn) | (direction == RightHandTurn));

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
    Q_ASSERT((direction == LeftHandTurn) | (direction == RightHandTurn));

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
    Q_ASSERT((direction == LeftHandTurn) | (direction == RightHandTurn));

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
