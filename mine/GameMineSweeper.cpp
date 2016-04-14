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
#include "SafePiece.h"
#include "MinePiece.h"
#include "WallPiece.h"
#include "GraduallyDrawer.h"

#include <QTime>
#include <QDebug>

namespace MineSweeper {

GameMineSweeper::GameMineSweeper(const QPixmap &sourcePixmap, const QSize &fieldSize, const QSize &xyCount, int bombCount, bool isAutoLock, QObject *parent) :
    IGame(parent),
    sourcePixmap(sourcePixmap),
    backBuffer(QPixmap(fieldSize)),
    mineCount(bombCount),
    isAutoLock(isAutoLock),
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
        emit informationUpdated();

        return;
    }

    if (safePieceCount<int>() == opendCount)
        return;

    int pieceW = backBuffer.width()  / xy.width();
    int pieceH = backBuffer.height() / xy.height();

    int x = (cursorPos.x() / pieceW) + 1;
    int y = (cursorPos.y() / pieceH) + 1;

    if (pieces[y][x]->isOpen() || pieces[y][x]->isLock())
        return;

    pieces[y][x]->open();
    changedPositions << QPoint(x, y);

    pieces[y][x]->isMine() ? ++missedCount
                           : ++opendCount;

    if (!pieces[y][x]->isNearMine())
        openChaining(x, y);

    if (isAutoLock)
        checkMinesForLock();

    emit screenUpdated();
    emit informationUpdated();

    if (opendCount == safePieceCount<int>() && missedCount == 0) {
        drawer = std::make_shared<GraduallyDrawer>(sourcePixmap, GraduallyDrawer::TopToBottom, 200);
        connect(&timer, SIGNAL(timeout()), this, SIGNAL(screenUpdated()));
        timer.setSingleShot(false);
        timer.start(16);
    }
}

void GameMineSweeper::draw(QPainter &dest)
{
    drawChanged();

    dest.drawPixmap(0, 0, backBuffer);

    if (drawer)
        drawer->draw(dest);
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

    destRect.setBottom((destRect.height() * missedCount) / 10 - 1);

    dest.fillRect(destRect, Qt::black);

    dest.setOpacity(opendCount / safePieceCount<double>());
    dest.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    destRect.setTop(destRect.bottom() + 1);
    destRect.setBottom(defaultBottom);

    QRect sourceRect = sourcePixmap.rect();
    sourceRect.setTop((sourceRect.height() * missedCount) / 10);

    dest.drawPixmap(destRect, sourcePixmap, sourceRect);
}

QString GameMineSweeper::shortInformation() const
{
    int safeCount = safePieceCount<int>();
    return QString("MineSweeper %1/%2 %3% opend.  %4 missed.").arg(opendCount)
                                                              .arg(safeCount)
                                                              .arg((opendCount * 100.0) / safeCount, 0, 'f', 2)
                                                              .arg(missedCount);
}

QPixmap GameMineSweeper::pixmap() const
{
    return sourcePixmap;
}

void GameMineSweeper::initPieces()
{
    QTime timer;
    timer.start();

    pieces.clear();

    int cx = xy.width()  + 2;
    int cy = xy.height() + 2;

    for (int y = 0; y < cy; ++y)
        pieces << QVector<MinePiecePointer>(cx, nullptr);

    createWallPieces();

    qDebug() << timer.elapsed() << "end of wall";

    createMinePieces();

    qDebug() << timer.elapsed() << "end of mine";

    createSafePieces();

    qDebug() << timer.elapsed() << "end of safe";
}

void GameMineSweeper::createWallPieces()
{
    int cx = xy.width()  + 2;
    int cy = xy.height() + 2;

    Q_ASSERT(pieces.size() == cy);

    auto wallPiece = std::make_shared<WallPiece>();

    for (int x = 0; x < cx; ++x) {
        pieces[     0][x] = wallPiece;
        pieces[cy - 1][x] = wallPiece;
    }

    for (int y = 1; y < cy - 1; ++y) {
        pieces[y][     0] = wallPiece;
        pieces[y][cx - 1] = wallPiece;
    }
}

void GameMineSweeper::createMinePieces()
{
    Q_ASSERT(pieces.size() == xy.height() + 2);

    notLockedMinesPos.clear();

    QSize pieceSize(backBuffer.width() / xy.width(), backBuffer.height() / xy.height());

    int halfX = xy.width()  / 2 + 1;
    int halfY = xy.height() / 2 + 1;
    int numOfCreatedMines = mineCount / 4;

    createMinePieces(        1, halfX,              1, halfY,       numOfCreatedMines);
    createMinePieces(halfX + 1, xy.width(),         1, halfY,       numOfCreatedMines);
    createMinePieces(        1, halfX,      halfY + 1, xy.height(), numOfCreatedMines);
    createMinePieces(halfX + 1, xy.width(), halfY + 1, xy.height(), numOfCreatedMines);

    int remian = mineCount % 4;

    for (int i = 0; i < remian; ++i) {
        int x = (mt() % xy.width())  + 1;
        int y = (mt() % xy.height()) + 1;

        if (pieces[y][x] == nullptr) {
            auto minePiece = std::make_shared<MinePiece>(pieceSize);

            pieces[y][x] = minePiece;
            notLockedMinesPos << QPoint(x, y);
        } else {
            --i;
        }
    }
}

void GameMineSweeper::createMinePieces(int minX, int maxX, int minY, int maxY, int numberOfMines)
{
    if (numberOfMines == 0)
        return;

    const int width  = maxX - minX + 1;
    const int height = maxY - minY + 1;
    const int numberOfPieces = width * height;

    const QSize pieceSize(backBuffer.width() / xy.width(), backBuffer.height() / xy.height());

    QList<int> indexes;
    indexes.reserve(numberOfPieces);

    for (int i = 0; i < numberOfPieces; ++i)
        indexes << i;

    for (int i = numberOfPieces - 1; i >= 0; --i) {
        int randIndex = mt() % (i + 1);
        int index = indexes[randIndex];
        std::swap(indexes[randIndex], indexes[i]);

        int x = index % width + minX;
        int y = index / width + minY;

        Q_ASSERT(pieces[y][x] == nullptr);

        auto minePiece = std::make_shared<MinePiece>(pieceSize);

        pieces[y][x] = minePiece;
        notLockedMinesPos << QPoint(x, y);

        if (numberOfMines + i == numberOfPieces)
            break;
    }
}

void GameMineSweeper::createSafePieces()
{
    int cx = xy.width()  + 2;
    int cy = xy.height() + 2;

    Q_ASSERT(pieces.size() == cy);

    QSize pieceSize(backBuffer.width()  / xy.width(), backBuffer.height() / xy.height());
    QRect sourceRect(QPoint(0, 0), pieceSize);

    for (int y = 1; y < cy - 1; ++y) {
        for (int x = 1; x < cx - 1; ++x) {
            if (pieces[y][x] != nullptr)
                continue;

            sourceRect.moveLeft(pieceSize.width() * (x - 1));

            auto arounds = getAroundPieces(x, y);

            int numberOfAroundMines = std::count_if(arounds.begin(), arounds.end(), [](MinePiecePointer &piece) {
                return piece != nullptr ? piece->isMine()
                                        : false;
            });

            auto safePiece = std::make_shared<SafePiece>(numberOfAroundMines, sourcePixmap, sourceRect);

            safePiece->setOpenPieceOpacity(0.5);
            pieces[y][x] = safePiece;
        }

        sourceRect.moveTopLeft(QPoint(0, sourceRect.top() + pieceSize.height()));
    }
}

QList<GameMineSweeper::MinePiecePointer> GameMineSweeper::getAroundPieces(int x, int y) const
{
    Q_ASSERT(x >= 1 && x < xy.width()  + 1);
    Q_ASSERT(y >= 1 && y < xy.height() + 1);

    QList<MinePiecePointer> result = {
        pieces[y - 1][x - 1],
        pieces[y - 1][x    ],
        pieces[y - 1][x + 1],
        pieces[y    ][x - 1],
        pieces[y    ][x + 1],
        pieces[y + 1][x - 1],
        pieces[y + 1][x    ],
        pieces[y + 1][x + 1],
    };

    return result;
}

QList<QPoint> GameMineSweeper::getAroundPositions(const QPoint &pos) const
{
    Q_ASSERT(pos.x() >= 1 && pos.x() < xy.width()  + 1);
    Q_ASSERT(pos.y() >= 1 && pos.y() < xy.height() + 1);

    int x = pos.x();
    int y = pos.y();

    QList<QPoint> result = {
        QPoint(x - 1, y - 1),
        QPoint(x - 1, y    ),
        QPoint(x - 1, y + 1),
        QPoint(x    , y - 1),
        QPoint(x    , y + 1),
        QPoint(x + 1, y - 1),
        QPoint(x + 1, y    ),
        QPoint(x + 1, y + 1),
    };

    return result;
}

GameMineSweeper::MinePiecePointer &GameMineSweeper::getPiece(const QPoint &pos)
{
    Q_ASSERT(pos.x() >= 0 && pos.x() < xy.width()  + 2);
    Q_ASSERT(pos.y() >= 0 && pos.y() < xy.height() + 2);

    return pieces[pos.y()][pos.x()];
}

void GameMineSweeper::drawAll()
{
    QTime time;
    time.start();

    QPainter painterBuffer(&backBuffer);

    for (int y = 1, limY = xy.height() + 1; y < limY; ++y) {
        for (int x = 1, limX = xy.width() + 1; x < limX; ++x) {
            drawPiece(painterBuffer, x, y);
        }
    }

    qDebug() << "drawAll" << time.elapsed();
}

void GameMineSweeper::drawChanged()
{
    if (changedPositions.isEmpty())
        return;

    QPainter painterBuffer(&backBuffer);

    for (auto &pos : changedPositions)
        drawPiece(painterBuffer, pos.x(), pos.y());

    changedPositions.clear();
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

                auto &piece = getPiece(checkPos);

                if (piece->isOpen() || piece->isMine())
                    continue;

                piece->open();
                changedPositions << checkPos;
                ++opendCount;

                if (!piece->isNearMine())
                    mustCheck << checkPos;
            }
        }
    }

    double opacity = opendCount / safePieceCount<double>() / 2.0 + 0.5;

    for (int y = 1, limY = xy.height() + 1; y < limY; ++y) {
        for (int x = 1, limX = xy.width() + 1; x < limX; ++x) {
            auto piece = pieces[y][x]/*.get()*/;

            if (!piece->isNearMine() && piece->isOpen()) {
                piece->setOpenPieceOpacity(opacity);
                changedPositions << QPoint(x, y);
            }
        }
    }

    std::sort(changedPositions.begin(), changedPositions.end(), [](const QPoint &lhs, const QPoint &rhs) {
        if (lhs.x() != rhs.x())
            return lhs.x() < rhs.x();

        return lhs.y() < rhs.y();
    });

    changedPositions.erase(std::unique(changedPositions.begin(), changedPositions.end()), changedPositions.end());
}

void GameMineSweeper::checkMinesForLock()
{
    QList<QPoint> lockedPostions;

    int i = 0;
    int max = notLockedMinesPos.size();

    for (; i < max; ++i) {
        auto &mine = getPiece(notLockedMinesPos.at(i));

        if (mine->isLock()) {
            notLockedMinesPos.removeAt(i--);
            --max;

            continue;
        }

        auto openedPiecesPos = getAroundPositions(notLockedMinesPos.at(i));

        auto itr = std::remove_if(openedPiecesPos.begin(), openedPiecesPos.end(), [&](const QPoint &aroundPos) {
            auto &piece = getPiece(aroundPos);

            return !piece->isOpen() || piece->numberOfAroundMines() == 0;
        });
        openedPiecesPos.erase(itr, openedPiecesPos.end());

        for (const auto &openedPos : openedPiecesPos) {
            auto aroundPoses = getAroundPositions(openedPos);

            int numberOfOpened = std::count_if(aroundPoses.begin(), aroundPoses.end(), [&](const QPoint &aroundPos) {
                auto &piece = getPiece(aroundPos);

                return !piece->isMine() && piece->isOpen();
            });

            auto centerPiece = getPiece(openedPos);

            if (numberOfOpened == 8 - centerPiece->numberOfAroundMines()) {
                for (auto &aroundPos : aroundPoses) {
                    auto &piece = getPiece(aroundPos);

                    if (piece->isMine()) {
                        piece->lock();
                        lockedPostions << aroundPos;
                    }
                }
            }
        }

        if (mine->isLock()) {
            notLockedMinesPos.removeAt(i--);
            --max;
        }
    }

    std::sort(lockedPostions.begin(), lockedPostions.end(), [](const QPoint &lhs, const QPoint &rhs) {
        if (lhs.x() != rhs.x())
            return lhs.x() < rhs.x();

        return lhs.y() < rhs.y();
    });

    lockedPostions.erase(std::unique(lockedPostions.begin(), lockedPostions.end()), lockedPostions.end());

    changedPositions += lockedPostions;
}

} // MineSweeper
