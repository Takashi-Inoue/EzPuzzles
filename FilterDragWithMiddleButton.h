/*
 * Copyright 2021 Takashi Inoue
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

#ifndef FILTERDRAGWITHMIDDLEBUTTON_H
#define FILTERDRAGWITHMIDDLEBUTTON_H

#include <QObject>
#include <QPoint>

class QMouseEvent;
class QScrollArea;

class FilterDragWithMiddleButton : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    bool onMouseMove(QScrollArea *scrollArea, QMouseEvent *event);
    bool onMousePress(QScrollArea *scrollArea, QMouseEvent *event);

    using CursorSliderPos = std::pair<QPoint, QPoint>;

    CursorSliderPos m_basePositions;
};

#endif // FILTERDRAGWITHMIDDLEBUTTON_H
