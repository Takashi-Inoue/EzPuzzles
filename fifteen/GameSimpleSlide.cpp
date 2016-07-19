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
#include "GameSimpleSlide.h"
#include "SimplePiecesFactory.h"
#include "SlideBlankPiece.h"
#include "FifteenSlideShuffler.h"
#include "FifteenPieceMover.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

namespace Fifteen {

QString GameSimpleSlide::gameName()
{
    return "Simple Slide";
}

GameSimpleSlide::GameSimpleSlide(const SourceImage &sourceImg, const QSize &xy, const QPoint &blankPos) :
    GameLikeFifteen(sourceImg, xy, new SlideShuffler(pieces, boardInfo, this->blankPos)),
    slideAnimationFrames(20),
    defaultBlankPos(blankPos),
    blankPos(blankPos)
{
    Q_ASSERT(!sourceImg.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(blankPos.x() >= 0 && blankPos.x() < xy.width());
    Q_ASSERT(blankPos.y() >= 0 && blankPos.y() < xy.height());

    initPieces();
    initBlankPiece();
}

IGame *GameSimpleSlide::cloneAsNewGame() const
{
    auto game = new GameSimpleSlide(sourceImg, boardInfo->boardSize(), defaultBlankPos);

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void GameSimpleSlide::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameId.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    // 以下の書き込み順はこの2関数に影響するので注意すること
    // GameSimpleSlide::load()
    // SimpleSlideGameInfo::load()

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << boardInfo->boardSize();
    stream << defaultBlankPos;
    stream << blankPos;
    stream << isStarted;
    stream << sourceImg.pixmap;

    QList<QPoint> defaultPositions;

    for (const auto &piece : pieces)
        defaultPositions << piece->pos().defaultPos();

    stream << defaultPositions;

    saveScreenshot(saveDirPath, screenshotSize);
}

bool GameSimpleSlide::load(const QString &loadFilePath)
{
    QFile file(loadFilePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);

    QString name;

    stream >> name;

    if (name != gameName())
        return false;

    gameId = GameID::fromQString(QFileInfo(loadFilePath).baseName());

    QSize xy;

    stream >> sourceImg.fullPath;
    stream >> xy;
    stream >> defaultBlankPos;
    stream >> blankPos;
    stream >> isStarted;
    stream >> sourceImg.pixmap;

    boardInfo = std::make_shared<BoardInformation>(xy, sourceImg.size());

    QList<QPoint> defaultPositions;
    stream >> defaultPositions;

    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces(defaultPositions);

    initBlankPiece();

    return true;
}

QString GameSimpleSlide::shortInformation() const
{
    return gameName();
}

void GameSimpleSlide::click(const QPoint &posInArray)
{
    if (posInArray == blankPos)
        return;

    if ((posInArray.x() != blankPos.x()) & (posInArray.y() != blankPos.y()))
        return;

    changedPos = posInArray.x() == blankPos.x() ? PieceMover(pieces, boardInfo->xCount()).slideVertical(blankPos, posInArray)
                                                : PieceMover(pieces, boardInfo->xCount()).slideHorizontal(blankPos, posInArray);

    blankPos = posInArray;
}

void GameSimpleSlide::initPieces()
{
    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces();
}

void GameSimpleSlide::initBlankPiece()
{
    pieces[blankPos.y() * boardInfo->xCount() + blankPos.x()] = std::make_shared<SlideBlankPiece>(boardInfo, blankPos, Qt::black, slideAnimationFrames);
}

GameSimpleSlide::GameSimpleSlide() :
    GameLikeFifteen(new SlideShuffler(pieces, boardInfo, this->blankPos)),
    slideAnimationFrames(20)
{
}

} // Fifteen
