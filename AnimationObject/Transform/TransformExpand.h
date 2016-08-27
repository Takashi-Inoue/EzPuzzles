/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TRANSFORMEXPAND_H
#define TRANSFORMEXPAND_H

#include "AbstractTransform.h"

namespace Transform {

class Expand : public AbstractTransform
{
public:
    enum ExpandType {
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
        HorizontalToCenter,
        HorizontalFromCenter,
        VerticalToCenter,
        VerticalFromCenter,
    };

    Expand(ExpandType expandType, int totalFrameCount);
    ~Expand() = default;

    void start(const QSizeF &base) override;
    QTransform transform() const override;

protected:
    typedef QPair<double, double> Scale;

    Scale calcScale() const;
    QPointF calcPos(const Scale &scale) const;

    ExpandType expandType;
    QSizeF baseSize;
};

} // Transform

#endif // TRANSFORMEXPAND_H
