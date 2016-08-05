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
#include "MineSweeperSaveData.h"
#include "MineLocker.h"
#include "MinePiecesFactory.h"
#include "GraduallyDrawer.h"
#include "SourceImage.h"
#include "Utility.h"

#include <QFile>
#include <QFileInfo>
#include <QDataStream>

#include <QDateTime>
#include <QTime>
#include <QDebug>

namespace MineSweeper {

QString GameMineSweeper::gameName()
{
    return "MineSweeper";
}

GameMineSweeper::GameMineSweeper(const SourceImage &sourceImage, const QSize &xyCount, int mineCount, bool isAutoLock) :
    sourceImg(sourceImage),
    backBuffer(QPixmap(sourceImage.size())),
    mineCount(mineCount),
    xy(xyCount),
    openedCount(0),
    missedCount(0),
    isInited(true)
{
    Q_ASSERT(!sourceImage.isNull());
    Q_ASSERT(!xyCount.isEmpty());
    Q_ASSERT(mineCount > 0 && mineCount < xyCount.width() * xyCount.height());

    if (isAutoLock)
        mineLocker = std::make_unique<MineLocker>(pieces);

    initPieces();
    drawAll();
}

GameID GameMineSweeper::gameID() const
{
    return gameId;
}

IGame *GameMineSweeper::cloneAsNewGame() const
{
    auto game = new GameMineSweeper(sourceImg, xy, mineCount, mineLocker != nullptr);

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void GameMineSweeper::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameId.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    // 以下の書き込み順は、この2関数に影響するので注意すること
    // GameMineSweeper::load()
    // MineSweeperGameInfo::load()

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << xy;
    stream << (mineLocker != nullptr);
    stream << mineCount;
    stream << openedCount;
    stream << missedCount;
    stream << PiecesFactory::toIntList(pieces);
    stream << sourceImg.pixmap;

    saveScreenshot(saveDirPath, screenshotSize);
}

bool GameMineSweeper::load(const QString &loadPath)
{
    QFile file(loadPath);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    // GameMineSweeper::save() の書き込み順と同調すること

    QDataStream stream(&file);

    QString name;

    stream >> name;

    if (name != gameName())
        return false;

    gameId = GameID::fromQString(QFileInfo(loadPath).baseName());

    QString sourceImgFullPath;
    bool isAutoLock;
    QList<int> intList;
    QPixmap sourceImgPixmap;

    stream >> sourceImgFullPath;
    stream >> xy;
    stream >> isAutoLock;
    stream >> mineCount;
    stream >> openedCount;
    stream >> missedCount;
    stream >> intList;
    stream >> sourceImgPixmap;

    sourceImg.fullPath = sourceImgFullPath;
    sourceImg.pixmap = sourceImgPixmap;

    backBuffer = QPixmap(sourceImg.size());

    QSize pieceSize(backBuffer.width() / xy.width(), backBuffer.height() / xy.height());

    PiecesFactory factory(sourceImg.pixmap, pieceSize, xy, mineCount, isAutoLock);

    pieces = factory.toPieces(intList);

    if (isAutoLock) {
        mineLocker = std::make_unique<MineLocker>(pieces);
        mineLocker->addMinesPositions(factory.getMinesPositions());
    }

    drawAll();

    isInited = true;

    return true;
}

void GameMineSweeper::onTickFrame()
{

}

void GameMineSweeper::click(const QSize &/*fieldSize*/, const QPoint &cursorPos)
{
    Q_ASSERT(isInited);
//    if (!isStarted) {
//        initPieces();

//        emit screenUpdated();
//        emit informationUpdated();

//        return;
//    }

    if (safePieceCount<int>() == openedCount)
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
                           : ++openedCount;

    if (!pieces[y][x]->isNearMine())
        openChaining(x, y);

    if (mineLocker != nullptr)
        changedPositions += mineLocker->lockMines();

    emit informationUpdated();

    if (openedCount == safePieceCount<int>() && missedCount == 0) {
        drawer = std::make_shared<GraduallyDrawer>(sourceImg.pixmap, GraduallyDrawer::TopToBottom, 200);
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
    QRect destRect(QPoint(0, 0), sourceImg.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio));

    if (missedCount >= 10) {
        dest.fillRect(destRect, Qt::black);
        return;
    }

    int defaultBottom = destRect.bottom();

    destRect.setBottom((destRect.height() * missedCount) / 10 - 1);

    dest.fillRect(destRect, Qt::black);

    dest.setOpacity(openedCount / safePieceCount<double>());
    dest.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    destRect.setTop(destRect.bottom() + 1);
    destRect.setBottom(defaultBottom);

    QRect sourceRect = sourceImg.rect();
    sourceRect.setTop((sourceRect.height() * missedCount) / 10);

    dest.drawPixmap(destRect, sourceImg.pixmap, sourceRect);
}

QString GameMineSweeper::shortInformation() const
{
    return "MineSweeper " + openedDescription();
}

const SourceImage &GameMineSweeper::sourceImage() const
{
    return sourceImg;
}

void GameMineSweeper::initPieces()
{
    QSize pieceSize(backBuffer.width() / xy.width(), backBuffer.height() / xy.height());

    PiecesFactory factory(sourceImg.pixmap, pieceSize, xy, mineCount, mineLocker != nullptr);

    pieces = factory.createPieces();

    if (mineLocker != nullptr)
        mineLocker->addMinesPositions(factory.getMinesPositions());
}

MinePiecePointer &GameMineSweeper::getPiece(const QPoint &pos)
{
    Q_ASSERT(pos.x() >= 0 && pos.x() < xy.width()  + 2);
    Q_ASSERT(pos.y() >= 0 && pos.y() < xy.height() + 2);

    return pieces[pos.y()][pos.x()];
}

const MinePiecePointer &GameMineSweeper::getPiece(const QPoint &pos) const
{
    Q_ASSERT(pos.x() >= 0 && pos.x() < xy.width()  + 2);
    Q_ASSERT(pos.y() >= 0 && pos.y() < xy.height() + 2);

    return pieces.at(pos.y()).at(pos.x());
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
                ++openedCount;

                if (!piece->isNearMine())
                    mustCheck << checkPos;
            }
        }
    }

    double opacity = openedCount / safePieceCount<double>() / 2.0 + 0.5;

    for (int y = 1, limY = xy.height() + 1; y < limY; ++y) {
        for (int x = 1, limX = xy.width() + 1; x < limX; ++x) {
            auto &piece = pieces[y][x];

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

QString GameMineSweeper::openedDescription() const
{
    int safeCount = safePieceCount<int>();

    return QString("%1/%2 %3% opend, %4 missed").arg(openedCount)
                                                .arg(safeCount)
                                                .arg((openedCount * 100.0) / safeCount, 0, 'f', 2)
            .arg(missedCount);
}

void GameMineSweeper::saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = saveDirPath + "/" + gameId.toString() + ".png";

    backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(ssPath, "PNG");
}

GameMineSweeper::GameMineSweeper() :
    mineCount(0),
    mineLocker(nullptr),
    openedCount(0),
    missedCount(0),
    isInited(false)
{
}

} // MineSweeper
