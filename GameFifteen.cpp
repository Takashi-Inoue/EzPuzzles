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
#include "GameFifteen.h"
#include "SplitPainter.h"
#include "SlideShuffler.h"
#include "PieceMover.h"

#include <QPainter>
#include <QDebug>

GameFifteen::GameFifteen(const QPixmap &sourcePixmap, const QSize &xyCount, QObject *parent) :
    PiecesGame(sourcePixmap, xyCount, new SlideShuffler(pieces, xyCount, QPoint(xyCount.width() - 1, xyCount.height() - 1)), parent),
    blankPos(-1, -1)
{
}

QString GameFifteen::shortInformation() const
{
    return "Slide";
}

void GameFifteen::clickOperation(const QSize &fieldSize, const QPoint &cursorPos)
{
    if (blankPos.x() == -1) {
        int blankIndex = pieces.indexOf(size.width() * size.height() - 1);
        blankPos = QPoint(blankIndex % size.width(), blankIndex / size.width());
    }

    changedIndex.clear();

    QList<double> splitterXs = SplitPainter::verticalSplitterPos(fieldSize.width(), size.width() - 1);
    QList<double> splitterYs = SplitPainter::horizontalSplitterPos(fieldSize.height(), size.height() - 1);

    QPoint pos(std::distance(splitterXs.begin(), std::upper_bound(splitterXs.begin(), splitterXs.end(), cursorPos.x())),
               std::distance(splitterYs.begin(), std::upper_bound(splitterYs.begin(), splitterYs.end(), cursorPos.y())));

    if (pos == blankPos)
        return;

    if (pos.x() == blankPos.x()) {
        changedIndex = PieceMover(pieces, size).slideVertical(blankPos, pos);
        blankPos = pos;
    } else if (pos.y() == blankPos.y()) {
        changedIndex = PieceMover(pieces, size).slideHorizontal(blankPos, pos);
        blankPos = pos;
    }
}

void GameFifteen::drawPiece(QPainter &painterBuffer, int pieceIndex)
{
    int originalPos = pieces.at(pieceIndex);

    if (originalPos == size.width() * size.height() - 1) {
        drawBlackPiece(painterBuffer, pieceIndex);
        return;
    }

    PiecesGame::drawPiece(painterBuffer, pieceIndex);
}
