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
#ifndef UNIQUEPOSITION_H
#define UNIQUEPOSITION_H

#include <QDataStream>
#include <QList>
#include <QPoint>
#include <QSize>

class UniquePosition
{
public:
    UniquePosition() = default;
    virtual ~UniquePosition() = default;

    void randomSelect(const QSize &xyCount);
    void select(const QPoint &pos);
    const QPoint &selectedPosition() const;
    bool isRandom() const;

    void read(QDataStream &stream);
    void write(QDataStream &stream) const;

    QString toString() const;

protected:
    bool m_isRandomSelect = false;
    QPoint m_selectedPos;
};

#endif // UNIQUEPOSITION_H
