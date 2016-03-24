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

#include "GameMineSweeper.h"
#include "EmptyPiece.h"
#include "MinePiece.h"
#include "WallPiece.h"

#include <QDebug>

namespace MineSweeper {

GameMineSweeper::GameMineSweeper(const QPixmap &sourcePixmap, const QSize &fieldSize, const QSize &xyCount, int bombCount, QObject *parent) :
    IGame(parent),
    sourcePixmap(sourcePixmap),
    backBuffer(QPixmap(fieldSize)),
    mineCount(bombCount),
    xy(xyCount),
    opendCount(0),
    missedCount(0),
    isStarted(true),
    mt(std::random_device()())
{
    initPieces();
    drawAll();
}

void GameMineSweeper::click(const QSize &/*fieldSize*/, const QPoint &cursorPos)
{
    if (!isStarted) {
        initPieces();

        emit screenUpdated();

        return;
    }

    if (emptyPieceCount<int>() == opendCount)
        return;

    int pieceW = backBuffer.width()  / xy.width();
    int pieceH = backBuffer.height() / xy.height();

    int x = (cursorPos.x() / pieceW) + 1;
    int y = (cursorPos.y() / pieceH) + 1;

    if (pieces[y][x]->isOpen())
        return;

    pieces[y][x]->open();
    changed << QPoint(x, y);

    pieces[y][x]->isMine() ? ++missedCount
                           : ++opendCount;

    if (!pieces[y][x]->isNearMine())
        openChaining(x, y);

    emit screenUpdated();
}

void GameMineSweeper::draw(QPainter &dest)
{
    drawChanged();

    dest.drawPixmap(0, 0, backBuffer);
}

QSize GameMineSweeper::maxFieldSize() const
{
    return backBuffer.size();
}

void GameMineSweeper::drawFinalImage(QPainter &dest) const
{
    QRect destRect(QPoint(0, 0), sourcePixmap.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio));

    if (missedCount >= 10) {
        dest.fillRect(destRect, Qt::black);
        return;
    }

    int defaultBottom = destRect.bottom();

    destRect.setBottom((destRect.height() * missedCount) / 10);

    dest.fillRect(destRect, Qt::black);

    dest.setOpacity(opendCount / emptyPieceCount<double>());

    destRect.setTop(destRect.bottom() + 1);
    destRect.setBottom(defaultBottom);

    QRect sourceRect = sourcePixmap.rect();
    sourceRect.setTop((sourceRect.height() * missedCount) / 10);

    dest.drawPixmap(destRect, sourcePixmap, sourceRect);
}

QString GameMineSweeper::shortInformation() const
{
    int emptyCount = emptyPieceCount<int>();
    return QString("MineSweeper %1/%2 %3% opend.  %4 missed.").arg(opendCount)
                                                              .arg(emptyCount)
                                                              .arg((opendCount * 100.0) / emptyCount, 0, 'f', 2)
            .arg(missedCount);
}

QPixmap GameMineSweeper::pixmap() const
{
    return sourcePixmap;
}

void GameMineSweeper::initPieces()
{
    int cx = xy.width() + 2;
    int cy = xy.height() + 2;

    QSize pieceSize(backBuffer.width()  / xy.width(), backBuffer.height() / xy.height());

    for (int y = 0; y < cy; ++y)
        pieces << QVector<std::shared_ptr<AbstractMinePiece>>(cx);

    auto wallPiece = std::make_shared<WallPiece>();

    wallPiece->setSize(pieceSize);
    wallPiece->open();

    for (int x = 0; x < cx; ++x) {
        pieces[     0][x] = wallPiece;
        pieces[cy - 1][x] = wallPiece;
    }

    for (int y = 1; y < cy - 1; ++y) {
        pieces[y][     0] = wallPiece;
        pieces[y][cx - 1] = wallPiece;
    }

    for (int i = 0; i < mineCount; ++i) {
        int x = (mt() % xy.width())  + 1;
        int y = (mt() % xy.height()) + 1;

        if (pieces[y][x] == nullptr) {
            auto minePiece = std::make_shared<MinePiece>();

            minePiece->setSize(pieceSize);
            pieces[y][x] = minePiece;
        } else {
            --i;
        }
    }

    QRect sourceRect(QPoint(0, 0), pieceSize);

    for (int y = 1; y < cy - 1; ++y) {
        for (int x = 1; x < cx - 1; ++x) {
            if (pieces[y][x] != nullptr)
                continue;

            sourceRect.moveLeft(pieceSize.width() * (x - 1));

            auto emptyPiece = std::make_shared<EmptyPiece>(sourcePixmap.copy(sourceRect), aroundMineCount(x, y));

            emptyPiece->setSize(pieceSize);
            emptyPiece->setOpenPieceOpacity(0.5);
            pieces[y][x] = emptyPiece;
        }

        sourceRect.moveTopLeft(QPoint(0, sourceRect.top() + pieceSize.height()));
    }
}

int GameMineSweeper::aroundMineCount(int x, int y) const
{
    Q_ASSERT(x >= 1 && x < xy.width()  + 1);
    Q_ASSERT(y >= 1 && y < xy.height() + 1);

    int count = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if ((dx | dy) == 0)
                continue;

            auto &piece = pieces[y + dy][x + dx];

            if (piece != nullptr && piece->isMine())
                ++count;
        }
    }

    return count;
}

void GameMineSweeper::drawAll()
{
    QPainter painterBuffer(&backBuffer);

    for (int y = 1, limY = xy.height() + 1; y < limY; ++y) {
        for (int x = 1, limX = xy.width() + 1; x < limX; ++x) {
            drawPiece(painterBuffer, x, y);
        }
    }
}

void GameMineSweeper::drawChanged()
{
    QPainter painterBuffer(&backBuffer);

    for (auto &pos : changed)
        drawPiece(painterBuffer, pos.x(), pos.y());

    changed.clear();
}

void GameMineSweeper::drawPiece(QPainter &painterBuffer, int x, int y)
{
    int pieceW = backBuffer.width()  / xy.width();
    int pieceH = backBuffer.height() / xy.height();

    pieces[y][x]->draw(painterBuffer, QPoint(pieceW * (x - 1), pieceH * (y - 1)));
}

void GameMineSweeper::openChaining(int x, int y)
{
    QList<QPoint> mustCheck;
    mustCheck << QPoint(x, y);

    while (!mustCheck.isEmpty()) {
        QPoint center = mustCheck.takeFirst();

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if ((dx | dy) == 0)
                    continue;

                QPoint checkPos(center + QPoint(dx, dy));

                auto &piece = pieces[checkPos.y()][checkPos.x()];

                if (piece->isOpen() || piece->isMine())
                    continue;

                piece->open();
                changed << checkPos;
                ++opendCount;

                if (!piece->isNearMine())
                    mustCheck << checkPos;
            }
        }
    }

    double opacity = opendCount / emptyPieceCount<double>() / 2.0 + 0.5;

    for (int y = 1, limY = xy.height() + 1; y < limY; ++y) {
        for (int x = 1, limX = xy.width() + 1; x < limX; ++x) {
            auto piece = pieces[y][x].get();

            if (!piece->isNearMine() && piece->isOpen()) {
                piece->setOpenPieceOpacity(opacity);
                changed << QPoint(x, y);
            }
        }
    }

    std::sort(changed.begin(), changed.end(), [](const QPoint &lhs, const QPoint &rhs) {
        if (lhs.x() != rhs.x())
            return lhs.x() < rhs.x();

        return lhs.y() < rhs.y();
    });

    changed.erase(std::unique(changed.begin(), changed.end()), changed.end());
}

} // MineSweeper
