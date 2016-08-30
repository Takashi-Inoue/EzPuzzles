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
#include "EffectSwapper.h"

namespace Fifteen {

EffectSwapper::EffectSwapper(PuzzlePiecePointer piece1, PuzzlePiecePointer piece2, int waitFrame) :
    AbstractAnimationObject(waitFrame, false),
    piece1(piece1),
    piece2(piece2)
{
    Q_CHECK_PTR(piece1);
    Q_CHECK_PTR(piece2);
    Q_ASSERT(waitFrame > 0);
}

bool EffectSwapper::onTickFrame()
{
    if (AbstractAnimationObject::onTickFrame())
        return true;

    if (piece1 == nullptr)
        return false;

    auto effect = piece1->effect();

    piece1->setEffect(piece2->effect());
    piece2->setEffect(effect);

    piece1.reset();
    piece2.reset();

    return false;
}

} // Fifteen
