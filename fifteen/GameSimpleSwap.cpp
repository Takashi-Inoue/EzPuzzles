﻿/*
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
#include "GameSimpleSwap.h"
#include "SimplePiecesFactory.h"
#include "FifteenSwapShuffler.h"
#include "FifteenPieceMover.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

namespace Fifteen {

QString GameSimpleSwap::gameName()
{
    return "Simple Swap";
}

GameSimpleSwap::GameSimpleSwap(const SourceImage &sourceImg, const QSize &xy, const QPoint &swapTargetPos) :
    GameLikeFifteen(sourceImg, xy, new SwapShuffler(pieces)),
    swapTargetPos(swapTargetPos)
{
    Q_ASSERT(!sourceImg.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(swapTargetPos.x() >= 0 && swapTargetPos.x() < xy.width());
    Q_ASSERT(swapTargetPos.y() >= 0 && swapTargetPos.y() < xy.height());

    initPieces();
}

IGame *GameSimpleSwap::cloneAsNewGame() const
{
    auto game = new GameSimpleSwap(sourceImg, xy, swapTargetPos);

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void GameSimpleSwap::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameId.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    // 以下の書き込み順はこの2関数に影響するので注意すること
    // GameSimpleSwap::load()
    // SimpleSwapGameInfo::load()

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << xy;
    stream << swapTargetPos;
    stream << isStarted;
    stream << sourceImg.pixmap;

    QList<QPoint> defaultPositions;

    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            defaultPositions << piece->defaultPos();
    }

    stream << defaultPositions;

    saveScreenshot(saveDirPath, screenshotSize);
}

bool GameSimpleSwap::load(const QString &loadFilePath)
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

    stream >> sourceImg.fullPath;
    stream >> xy;
    stream >> swapTargetPos;
    stream >> isStarted;
    stream >> sourceImg.pixmap;

    QList<QPoint> defaultPositions;
    stream >> defaultPositions;

    pieces = SimplePiecesFactory(sourceImg.pixmap, xy).createPieces(defaultPositions);

    return true;
}

QString GameSimpleSwap::shortInformation() const
{
    return gameName();
}

void GameSimpleSwap::click(const QPoint &posInArray)
{
    if (posInArray == swapTargetPos)
        return;

    auto &pieceClick  = pieces   [posInArray.y()][   posInArray.x()];
    auto &pieceTarget = pieces[swapTargetPos.y()][swapTargetPos.x()];

    pieceClick->swapPos(pieceTarget.get());
    pieceClick.swap(pieceTarget);

    changedPos.clear();
    changedPos << posInArray << swapTargetPos;
}

void GameSimpleSwap::initPieces()
{
    SimplePiecesFactory factory(sourceImg.pixmap, xy);

    pieces = factory.createPieces();
}

GameSimpleSwap::GameSimpleSwap() :
    GameLikeFifteen(new SwapShuffler(pieces))
{
}

} // Fifteen