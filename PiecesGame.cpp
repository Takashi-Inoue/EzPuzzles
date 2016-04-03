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
#include "PiecesGame.h"
#include "SplitPainter.h"
#include "IShuffler.h"
#include "PieceMover.h"

#include <QPainter>
#include <QDebug>

PiecesGame::PiecesGame(const QPixmap &sourcePixmap, const QSize &xyCount, IShuffler *shuffler, QObject *parent) :
    IGame(parent),
    size(xyCount),
    sourcePixmap(sourcePixmap),
    backBuffer(0, 0),
    isStarted(false),
    shuffler(shuffler)
{
    Q_CHECK_PTR(shuffler);

    for (int i = 0, count = xyCount.width() * xyCount.height(); i < count; ++i)
        pieces.push_back(i);

    calcSourceSplitterPos();

    connect(shuffler, &IShuffler::update, this, &PiecesGame::pieceChanged);
}

void PiecesGame::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    if (!isStarted) {
        isStarted = true;
        shuffler->exec();
        return;
    }

    clickOperation(fieldSize, cursorPos);

    if (isGameClear())
        gameOver();

    emit screenUpdated();
}

void PiecesGame::draw(QPainter &dest)
{
    QRect destRect = dest.viewport();

    if (backBuffer.size() != destRect.size()) {
        backBuffer = QPixmap(destRect.size());

        calcDestSplitterPos();
    }

    if (!isStarted) {
        drawAll();
    } else {
        drawChanged();

        if (!changedIndex.isEmpty()) {
            QPainter painterBuffer(&backBuffer);
            SplitPainter(QPen(Qt::darkGray, 1), size.width() - 1, size.height() - 1).draw(painterBuffer);
        }
    }

    dest.drawPixmap(0, 0, backBuffer);
}

QSize PiecesGame::maxFieldSize() const
{
    return sourcePixmap.size();
}

void PiecesGame::drawFinalImage(QPainter &dest) const
{
    QSize destSize = sourcePixmap.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio);
    dest.drawPixmap(QRect(QPoint(0, 0), destSize), sourcePixmap, sourcePixmap.rect());
}

QPixmap PiecesGame::pixmap() const
{
    return sourcePixmap;
}

void PiecesGame::clickOperation(const QSize &/*fieldSize*/, const QPoint &/*cursorPos*/)
{
    // empty
}

void PiecesGame::drawPiece(QPainter &painterBuffer, int pieceIndex)
{
    int originalPos = pieces.at(pieceIndex);

    QPoint destPos(pieceIndex % size.width(), pieceIndex / size.width());

    QPointF destTL(destSplitterXs.at(destPos.x()), destSplitterYs.at(destPos.y()));
    QPointF destBR(destSplitterXs.at(destPos.x() + 1), destSplitterYs.at(destPos.y() + 1));
    QRectF destRect(destTL, destBR);

    QPoint sourcePos(originalPos % size.width(), originalPos / size.width());

    QPointF sourceTL(sourceSplitterXs.at(sourcePos.x()), sourceSplitterYs.at(sourcePos.y()));
    QPointF sourceBR(sourceSplitterXs.at(sourcePos.x() + 1), sourceSplitterYs.at(sourcePos.y() + 1));
    QRectF sourceRect(sourceTL, sourceBR);

    painterBuffer.drawPixmap(destRect, sourcePixmap, sourceRect);
}

bool PiecesGame::isGameClear() const
{
    for (int i = 0, lim = pieces.size(); i < lim; ++i) {
        if (pieces.at(i) != i)
            return false;
    }

    return true;
}

void PiecesGame::gameOver()
{
    isStarted = false;
}

void PiecesGame::drawBlackPiece(QPainter &painterBuffer, int pieceIndex)
{
    QPoint destPos(pieceIndex % size.width(), pieceIndex / size.width());

    QPointF destTL(destSplitterXs.at(destPos.x()), destSplitterYs.at(destPos.y()));
    QPointF destBR(destSplitterXs.at(destPos.x() + 1), destSplitterYs.at(destPos.y() + 1));
    QRectF destRect(destTL, destBR);

    painterBuffer.fillRect(destRect, Qt::black);
}

void PiecesGame::pieceChanged(const QList<int> &changed)
{
    changedIndex = changed;

    emit screenUpdated();
}

void PiecesGame::drawAll()
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painterBuffer.drawPixmap(backBuffer.rect(), sourcePixmap, sourcePixmap.rect());
}

void PiecesGame::drawChanged()
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (int index : changedIndex)
        drawPiece(painterBuffer, index);
}

void PiecesGame::calcSourceSplitterPos()
{
    sourceSplitterXs.push_back(0);
    sourceSplitterYs.push_back(0);

    sourceSplitterXs << SplitPainter::verticalSplitterPos(sourcePixmap.width(), size.width() - 1) << sourcePixmap.width();
    sourceSplitterYs << SplitPainter::horizontalSplitterPos(sourcePixmap.height(), size.height() - 1) << sourcePixmap.height();
}

void PiecesGame::calcDestSplitterPos()
{
    destSplitterXs.clear();

    destSplitterXs.push_back(0);
    destSplitterYs.push_back(0);

    destSplitterXs << SplitPainter::verticalSplitterPos(backBuffer.width(), size.width() - 1) << backBuffer.width();
    destSplitterYs << SplitPainter::horizontalSplitterPos(backBuffer.height(), size.height() - 1) << backBuffer.height();
}

