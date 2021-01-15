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
#include "FifteenSwapShuffler.h"

#include <QRandomGenerator>

namespace Fifteen {

void SwapShuffler::exec()
{
    Q_ASSERT(!m_pieces.isEmpty());
    Q_ASSERT(m_boardInfo != nullptr);

    int countX = m_boardInfo->xCount();

    for (int i = int(m_pieces.size()) - 1; i > 0; --i) {
        int r = QRandomGenerator::global()->bounded(i);

        auto &lhs = m_pieces[i];
        auto &rhs = m_pieces[r];

        int lx = i % countX;
        int ly = i / countX;
        int rx = r % countX;
        int ry = r / countX;

        lhs.swap(rhs);
        lhs->setPosWithoutAnimation(QPoint(lx, ly));
        rhs->setPosWithoutAnimation(QPoint(rx, ry));
    }
}

} // Fifteen
