﻿/*
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

#include "AbstractSubWidget.h"
#include "Dragger.h"

#include <QRect>

class SubFrame : public AbstractSubWidget
{
public:
    SubFrame(const QSize &frameSize, const QSize &boundingSize, bool adjustContents = true
           , const QPoint &framePos = {0, 0});

    void mousePress(QMouseEvent *) override;
    void mouseRelease(QMouseEvent *) override;
    void mouseMove(QMouseEvent *) override;
    void mouseEnter(QEvent *) override {}
    void mouseLeave(QEvent *) override {}
    QPoint pos() const override;

protected:
    void drawImpl(QPainter &) override;

    void correctPosition();

    const QSize m_boundingSize;
    const bool m_adjustContents;

    QRect m_subFrameRect;
    QSize m_destSize;

    Dragger m_dragger;
};

#endif // SUBFRAME_H
