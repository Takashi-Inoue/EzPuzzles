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
#ifndef IPIECE_H
#define IPIECE_H

#include <QPainter>

class IPiece
{
public:
    IPiece() = default;
    virtual ~IPiece() = default;

    virtual void draw(QPainter &painter, const QPoint &pos) = 0;
    virtual void draw(QPainter &painter, const QPoint &pos, const QSize &targetSize) = 0;

private:
    IPiece(const IPiece &) = delete;
    IPiece(IPiece &&) = delete;
    IPiece &operator=(const IPiece &) = delete;
    IPiece &operator=(IPiece &&) = delete;
};

#endif // IPIECE_H
