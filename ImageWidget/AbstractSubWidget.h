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
#ifndef ABSTRACTSUBWIDGET_H
#define ABSTRACTSUBWIDGET_H

#include <QObject>
#include <QMouseEvent>

class QPainter;
class QPoint;

class AbstractSubWidget : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual void draw(QPainter &painter)
    {
        if (m_isVisible)
            drawImpl(painter);
    }

    virtual void mousePress(QMouseEvent *) = 0;
    virtual void mouseRelease(QMouseEvent *) = 0;
    virtual void mouseMove(QMouseEvent *) = 0;
    virtual void mouseEnter(QEvent *) = 0;
    virtual void mouseLeave(QEvent *) = 0;
    virtual QPoint pos() const = 0;

    virtual void setVisible(bool isVisible)
    {
        m_isVisible = isVisible;

        emit updated();
    }

signals:
    void updated();

protected:
    virtual void drawImpl(QPainter &) = 0;

    bool m_isVisible = true;
};

#endif // ABSTRACTSUBWIDGET_H
