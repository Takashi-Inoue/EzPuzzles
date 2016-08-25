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
#ifndef FINALIMAGE_H
#define FINALIMAGE_H

#include <QPainter>

#include <memory>

class FinalImage
{
public:
    FinalImage(QPixmap pixmap);
    virtual ~FinalImage() = default;

    virtual void draw(QPainter &painter);

protected:
    QPixmap pixmap;
};

typedef std::shared_ptr<FinalImage> FinalImagePointer;

#endif // FINALIMAGE_H