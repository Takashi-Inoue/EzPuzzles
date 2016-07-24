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
#include "SelectCellGrid.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"

#include <QGuiApplication>
#include <QPainter>
#include <QDebug>

SelectCellGrid::SelectCellGrid(int vCellCount, int hCellCount) :
    pen(QColor(255, 0, 0, 128), 1),
    gridLines(vCellCount - 1, hCellCount - 1),
    isPressed(false),
    onMousePos(invalidPoint()),
    selectedPos(vCellCount - 1, hCellCount - 1),
    isRandomSelect(false)
{
    Q_ASSERT(vCellCount >= 0 && hCellCount >= 0);
}

SelectCellGrid::SelectCellGrid(const QPen &pen, int vSplitterCount, int hSplitterCount) :
    pen(pen),
    gridLines(vSplitterCount, hSplitterCount),
    isPressed(false),
    onMousePos(invalidPoint()),
    selectedPos(vSplitterCount, hSplitterCount),
    isRandomSelect(false)
{
    Q_ASSERT(vSplitterCount >= 0 && hSplitterCount >= 0);
}

void SelectCellGrid::draw(QPainter &painter)
{
    QRectF rect = painter.clipBoundingRect();

    if (rect.isEmpty())
        rect = painter.viewport();

    gridLines.setRect(rect);

    painter.save();
    painter.setPen(pen);

    gridLines.draw(painter);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::transparent);

    if (onMousePos != invalidPoint()) {
        QRectF rect = gridLines.cellRect(onMousePos);

        if (isPressed) {
            painter.fillRect(rect, QColor(128, 128, 128, 64));
            Effect::SimpleFrame(2, QColor(0, 0, 0, 192), QColor(128, 128, 128, 0)).draw(painter, rect);
        } else {
            painter.fillRect(rect, QColor(224, 224, 224, 64));
            Effect::SimpleFrame(2, QColor(0, 0, 0, 192), QColor(192, 192, 192, 128)).draw(painter, rect);
        }
    }

    painter.setPen(Qt::black);

    if (isRandomSelect) {
        QRectF rect = painter.clipBoundingRect();

        if (rect.isEmpty())
            rect = painter.viewport();

        painter.fillRect(rect, Qt::DiagCrossPattern);
    } else if (selectedPos != invalidPoint()) {
        painter.fillRect(gridLines.cellRect(selectedPos), Qt::DiagCrossPattern);
    }

    painter.restore();
}

void SelectCellGrid::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    isPressed = gridLines.cellPosFromPixelPos(event->pos()) != invalidPoint();

    if (!isPressed)
        return;

    emit updated();
}

void SelectCellGrid::mouseRelease(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (!isPressed)
        return;

    if (onMousePos != invalidPoint())
        selectedPos = onMousePos;

    isPressed = false;

    emit updated();
}

void SelectCellGrid::mouseMove(QMouseEvent *event)
{
    QPoint cellPos = gridLines.cellPosFromPixelPos(event->pos());

    if (cellPos == onMousePos)
        return;

    onMousePos = cellPos;

    emit updated();
}

void SelectCellGrid::mouseLeave(QEvent *)
{
    onMousePos = invalidPoint();

    emit updated();
}

QPoint SelectCellGrid::pos() const
{
    return QPoint(0, 0);
}

void SelectCellGrid::setRandomSelect(bool isRandom)
{
    if (isRandomSelect == isRandom)
        return;

    isRandomSelect = isRandom;

    emit updated();
}

void SelectCellGrid::setCellCount(int vCellCount, int hCellCount)
{
    gridLines = GridLines(vCellCount - 1, hCellCount - 1);

    if (selectedPos.x() >= vCellCount)
        selectedPos.rx() = vCellCount - 1;

    if (selectedPos.y() >= hCellCount)
        selectedPos.ry() = hCellCount - 1;

    emit updated();
}

const QPoint &SelectCellGrid::selectedCellPos() const
{
    return selectedPos;
}

QPoint SelectCellGrid::invalidPoint() const
{
    return QPoint(-1, -1);
}
