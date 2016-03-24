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
#include "GameSwap.h"
#include "SplitPainter.h"
#include "SwapShuffler.h"
#include "PieceMover.h"

#include <QPainter>
#include <QDebug>

GameSwap::GameSwap(const QPixmap &sourcePixmap, const QSize &xyCount, QObject *parent) :
    PiecesGame(sourcePixmap, xyCount, new SwapShuffler(pieces, xyCount), parent)
{
}

QString GameSwap::shortInformation() const
{
    return "Swap";
}

void GameSwap::clickOperation(const QSize &fieldSize, const QPoint &cursorPos)
{
    changedIndex.clear();

    QList<double> splitterXs = SplitPainter::verticalSplitterPos(fieldSize.width(), size.width() - 1);
    QList<double> splitterYs = SplitPainter::horizontalSplitterPos(fieldSize.height(), size.height() - 1);

    QPoint pos(std::distance(splitterXs.begin(), std::upper_bound(splitterXs.begin(), splitterXs.end(), cursorPos.x())),
               std::distance(splitterYs.begin(), std::upper_bound(splitterYs.begin(), splitterYs.end(), cursorPos.y())));

    PieceMover(pieces, size).swap(QPoint(0, 0), pos);

    int index = pos.y() * size.width() + pos.x();

    changedIndex << 0 << index;
}
