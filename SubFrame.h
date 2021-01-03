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
#ifndef SUBFRAME_H
#define SUBFRAME_H

#include "ISubWidget.h"
#include "Dragger.h"

#include <QRect>

class SubFrame : public ISubWidget
{
public:
    SubFrame(const QRect &subFrameRect);
    SubFrame(const QRect &subFrameRect, bool adjustMaxWidth, bool adjustMaxHeight);

    void draw(QPainter &) override;
    void mousePress(QMouseEvent *) override;
    void mouseRelease(QMouseEvent *) override;
    void mouseMove(QMouseEvent *) override;
    void mouseEnter(QEvent *) override {}
    void mouseLeave(QEvent *) override {}
    QPoint pos() const override;
    QPoint posOnImage() const override;

protected:
    void correctPosition();
    void correctSize();

    QRect m_subFrameRect;
    QRect m_maxRect;
    Dragger m_dragger;

    QPair<bool, bool> m_sizeMaximized;
};

#endif // SUBFRAME_H
