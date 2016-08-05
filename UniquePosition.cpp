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
#include "UniquePosition.h"

UniquePosition::UniquePosition() :
    isRandomSelect(false),
    mt(std::random_device()())
{
}

void UniquePosition::randomSelect(const QSize &xyCount)
{
    Q_ASSERT(!xyCount.isEmpty());

    isRandomSelect = true;

    selectedPos.rx() = mt() % xyCount.width();
    selectedPos.ry() = mt() % xyCount.height();
}

void UniquePosition::select(const QPoint &pos)
{
    isRandomSelect = false;
    selectedPos = pos;
}

const QPoint &UniquePosition::selectedPosition() const
{
    return selectedPos;
}

void UniquePosition::read(QDataStream &stream)
{
    stream >> isRandomSelect;
    stream >> selectedPos;
}

void UniquePosition::write(QDataStream &stream) const
{
    stream << isRandomSelect;
    stream << selectedPos;
}

QString UniquePosition::toString() const
{
    return isRandomSelect ? "[???]<Random>"
                          : QString("[%1, %2]<Specified>").arg(selectedPos.x() + 1).arg(selectedPos.y() + 1);
}
