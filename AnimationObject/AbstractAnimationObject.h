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
#ifndef ABSTRACTANIMATIONOBJECT_H
#define ABSTRACTANIMATIONOBJECT_H

#include "IAnimationObject.h"

class AbstractAnimationObject : public IAnimationObject
{
public:
    AbstractAnimationObject(int totalFrameCount, bool isLoop);

    // IAnimationObject
    bool onTickFrame() override;
    void skipAnimation() override;
    bool isLoopAnimation() override;
    bool isFinishedAnimation() override;

protected:
    void resetFrame();
    double totalFrames() const;
    double nowFrame() const;

private:
    int m_totalFrameCount;
    int m_nowFrameCount;
    bool m_isLoop;
};

#endif // ABSTRACTANIMATIONOBJECT_H
