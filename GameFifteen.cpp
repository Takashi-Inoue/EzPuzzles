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

QString GameFifteen::savedataExtension()
{
    return "gft";
}

QString GameFifteen::gameName()
{
    return "Slide";
}

GameFifteen::GameFifteen(const SourceImage &sourceImage, const QSize &xyCount, QObject *parent) :
    PiecesGame(sourceImage, xyCount, new SlideShuffler(pieces, xyCount, QPoint(xyCount.width() - 1, xyCount.height() - 1)), parent),
    blankPos(-1, -1)
{
}

IGame *GameFifteen::cloneAsNewGame() const
{
    return new GameFifteen(sourceImage(), xy, parent());
}

QString GameFifteen::shortInformation() const
{
    return "Slide";
}

void GameFifteen::clickOperation(const QSize &fieldSize, const QPoint &cursorPos)
{
    if (blankPos.x() == -1) {
        int blankIndex = pieces.indexOf(xy.width() * xy.height() - 1);
        blankPos = QPoint(blankIndex % xy.width(), blankIndex / xy.width());
    }

    changedIndex.clear();

    QList<double> splitterXs = SplitPainter::verticalSplitterPos(fieldSize.width(), xy.width() - 1);
    QList<double> splitterYs = SplitPainter::horizontalSplitterPos(fieldSize.height(), xy.height() - 1);

    QPoint pos(std::distance(splitterXs.begin(), std::upper_bound(splitterXs.begin(), splitterXs.end(), cursorPos.x())),
               std::distance(splitterYs.begin(), std::upper_bound(splitterYs.begin(), splitterYs.end(), cursorPos.y())));

    if (pos == blankPos)
        return;

    if (pos.x() == blankPos.x()) {
        changedIndex = PieceMover(pieces, xy).slideVertical(blankPos, pos);
        blankPos = pos;
    } else if (pos.y() == blankPos.y()) {
        changedIndex = PieceMover(pieces, xy).slideHorizontal(blankPos, pos);
        blankPos = pos;
    }
}

void GameFifteen::drawPiece(QPainter &painterBuffer, int pieceIndex)
{
    int originalPos = pieces.at(pieceIndex);

    if (originalPos == xy.width() * xy.height() - 1) {
        drawBlackPiece(painterBuffer, pieceIndex);
        return;
    }

    PiecesGame::drawPiece(painterBuffer, pieceIndex);
}
