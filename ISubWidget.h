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
#ifndef ISUBWIDGET_H
#define ISUBWIDGET_H

#include <QObject>

class QPainter;
class QPoint;

class ISubWidget : public QObject
{
    Q_OBJECT
public:
    ISubWidget() = default;
    virtual ~ISubWidget() = default;

    virtual void draw(QPainter &) = 0;
    virtual void mousePress(const QPoint &) = 0;
    virtual void mouseRelease(const QPoint &) = 0;
    virtual void mouseMove(const QPoint &) = 0;
    virtual QPoint pos() const = 0;

signals:
    void updated();
};

#endif // ISUBWIDGET_H
