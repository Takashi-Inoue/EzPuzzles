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
#include "CellSelectionGrid.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"

#include <QGuiApplication>
#include <QPainter>
#include <QDebug>

CellSelectionGrid::CellSelectionGrid(ushort vCellCount, ushort hCellCount, QObject *parent)
    : ISubWidget(parent)
    , m_gridLines(vCellCount - 1, hCellCount - 1)
    , m_isPressed(false)
    , m_onMouseCell(invalidCell())
    , m_selectedCell(vCellCount - 1, hCellCount - 1)
    , m_isRandomSelect(false)
{
}

void CellSelectionGrid::draw(QPainter &painter)
{
    QRectF wholeRect = painter.clipBoundingRect();

    if (wholeRect.isEmpty())
        wholeRect = painter.viewport();

    m_gridLines.setRect(wholeRect);

    painter.save();
    painter.setPen(QColor(255, 0, 0, 128));

    m_gridLines.draw(painter);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (m_onMouseCell != invalidCell())
        drawOnMouseEffect(painter);

    painter.setPen(Qt::black);

    if (m_isRandomSelect)
        painter.fillRect(wholeRect, Qt::DiagCrossPattern);
    else if (m_selectedCell != invalidCell())
        painter.fillRect(m_gridLines.cellRect(m_selectedCell), Qt::DiagCrossPattern);

    painter.restore();
}

void CellSelectionGrid::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    m_isPressed = m_gridLines.cellPosFromPixelPos(event->pos()) != invalidCell();

    if (!m_isPressed)
        return;

    emit updated();
}

void CellSelectionGrid::mouseRelease(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (!m_isPressed)
        return;

    if (m_onMouseCell != invalidCell())
        m_selectedCell = m_onMouseCell;

    m_isPressed = false;

    emit updated();
}

void CellSelectionGrid::mouseMove(QMouseEvent *event)
{
    QPoint cellPos = m_gridLines.cellPosFromPixelPos(event->pos());

    if (cellPos == m_onMouseCell)
        return;

    m_onMouseCell = cellPos;

    emit updated();
}

void CellSelectionGrid::mouseLeave(QEvent *)
{
    m_onMouseCell = invalidCell();

    emit updated();
}

QPoint CellSelectionGrid::pos() const
{
    return QPoint(0, 0);
}

void CellSelectionGrid::setCellCount(ushort vCellCount, ushort hCellCount)
{
    m_gridLines = GridLines(vCellCount - 1, hCellCount - 1);

    if (uint(m_selectedCell.x()) >= vCellCount)
        m_selectedCell.rx() = vCellCount - 1;

    if (uint(m_selectedCell.y()) >= hCellCount)
        m_selectedCell.ry() = hCellCount - 1;

    emit updated();
}

const QPoint &CellSelectionGrid::selectedCellPos() const
{
    return m_selectedCell;
}

void CellSelectionGrid::setRandomSelection(bool isRandom)
{
    if (m_isRandomSelect == isRandom)
        return;

    m_isRandomSelect = isRandom;

    emit updated();
}

void CellSelectionGrid::drawOnMouseEffect(QPainter &painter)
{
    enum Parts {rect, borderOuter, borderInner};

    static const QHash<bool, QList<QColor>> hashColors = {
        {true,  {QColor(128, 128, 128, 64), QColor(0, 0, 0, 192), QColor(128, 128, 128, 0)}},
        {false, {QColor(224, 224, 224, 64), QColor(0, 0, 0, 192), QColor(192, 192, 192, 128)}},
    };

    const QRectF cellRect = m_gridLines.cellRect(m_onMouseCell);
    const QList<QColor> &colors = hashColors[m_isPressed];

    painter.fillRect(cellRect, colors[rect]);
    Effect::SimpleFrame(2, colors[borderOuter], colors[borderInner]).draw(painter, cellRect);
}
