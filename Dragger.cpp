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
#include "Dragger.h"

void Dragger::mouseDown(const QPoint &pos)
{
    m_pos = pos;
    m_isDragging = true;
}

void Dragger::mouseRelease(const QPoint &)
{
    m_isDragging = false;
}

void Dragger::mouseMove(const QPoint &pos)
{
    m_subPos = pos - m_pos;
    m_pos = pos;
}

const QPoint &Dragger::sub() const
{
    return m_subPos;
}

bool Dragger::isDragging() const
{
    return m_isDragging;
}
