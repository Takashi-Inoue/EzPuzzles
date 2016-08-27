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
#ifndef CHAINEDTRANSFORM_H
#define CHAINEDTRANSFORM_H

#include "AbstractTransform.h"

namespace Transform {

class ChainedTransform : public AbstractTransform
{
public:
    ChainedTransform();
    ~ChainedTransform() = default;

    void addTransform(TransformPointer transform);

    // IAnimationObject
    bool onTickFrame() override;
    void skipAnimation() override;
    bool isLoopAnimation() override;
    bool isFinishedAnimation() override;

    // AbstractTransform
    void start(const QSizeF &start) override;
    QTransform transform() const override;

protected:
    QList<TransformPointer> transforms;
    int index;
};

} // Transform

#endif // CHAINEDTRANSFORM_H
