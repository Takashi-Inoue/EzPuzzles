/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "GameSimpleSwap.h"
#include "SimplePiecesFactory.h"
#include "FifteenSwapShuffler.h"
#include "FifteenPieceMover.h"

#include <QDataStream>
#include <QFile>
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
    return nullptr; // 関数廃止予定
}

void GameSimpleSwap::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameID.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << xy;
    stream << isStarted;
    stream << swapTargetPos;
    stream << sourceImg.pixmap;

    saveScreenshot(saveDirPath, screenshotSize);
}

bool GameSimpleSwap::load(const QString &loadPath)
{
    return false;
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

} // Fifteen
