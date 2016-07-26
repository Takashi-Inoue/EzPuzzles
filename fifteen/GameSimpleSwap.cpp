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
#include "GameSimpleSwap.h"
#include "SimplePiecesFactory.h"
#include "FifteenSwapShuffler.h"
#include "FifteenPieceMover.h"
#include "AnimationObject/Animation/AnimationWarpMove.h"
#include "AnimationObject/Effect/CompositeEffect.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

extern QThread gameThread;

namespace Fifteen {

QString GameSimpleSwap::gameName()
{
    return "Simple Swap";
}

GameSimpleSwap::GameSimpleSwap(const SourceImage &sourceImg, const QSize &xy, const UniquePosition &swapTargetPos) :
    GameLikeFifteen(sourceImg, xy),
    swapTargetPos(swapTargetPos)
{
    Q_ASSERT(!sourceImg.isNull());
    Q_ASSERT(!xy.isEmpty());

    initPieces();
    setAnimationToPieces();
    createShuffler();
}

IGame *GameSimpleSwap::cloneAsNewGame() const
{
    auto game = new GameSimpleSwap(sourceImg, boardInfo->boardSize(), swapTargetPos);

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
    stream << boardInfo->boardSize();
    swapTargetPos.write(stream);
    stream << static_cast<qint8>(gamePhase);
    stream << sourceImg.pixmap;

    QList<QPoint> defaultPositions;

    for (const auto &piece : pieces)
        defaultPositions << piece->pos().defaultPos();

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

    QSize xy;
    qint8 phase;

    stream >> sourceImg.fullPath;
    stream >> xy;
    swapTargetPos.read(stream);
    stream >> phase;
    stream >> sourceImg.pixmap;

    gamePhase = static_cast<GamePhase>(phase);

    boardInfo = std::make_shared<BoardInformation>(xy, sourceImg.size());

    QList<QPoint> defaultPositions;
    stream >> defaultPositions;

    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces(defaultPositions);

    setAnimationToPieces();
    setGraduallyFrame();

    createShuffler();

    createBackBuffer();
    drawAllPieces();

    return true;
}

QString GameSimpleSwap::shortInformation() const
{
    return gameName();
}

void GameSimpleSwap::click(const QPoint &piecePos)
{
    if (piecePos == swapTargetPos.selectedPosition())
        return;

    auto &pieceClick  = getPiece(piecePos);
    auto &pieceTarget = getPiece(swapTargetPos.selectedPosition());

    pieceClick.swap(pieceTarget);
    pieceClick->setPos(piecePos);
    pieceTarget->setPos(swapTargetPos.selectedPosition());

    auto effect = pieceClick->effect();
    pieceClick->setEffect(pieceTarget->effect());
    pieceTarget->setEffect(effect);

    addChangedPieces({pieceClick, pieceTarget});
}

void GameSimpleSwap::createShuffler()
{
    Q_ASSERT(boardInfo != nullptr);

    shuffler  = std::make_unique<SwapShuffler>(pieces, boardInfo);

    connect(this, SIGNAL(startShuffle()), shuffler.get(), SLOT(start()));
    connect(shuffler.get(), SIGNAL(completed()), this, SLOT(onCompletedShuffling()));
    connect(shuffler.get(), SIGNAL(completed()), this, SLOT(setGraduallyFrame()));
    connect(shuffler.get(), SIGNAL(update(QList<PuzzlePiecePointer>)), this, SLOT(addChangedPieces(QList<PuzzlePiecePointer>)));

    shuffler->moveToThread(&gameThread);
}

void GameSimpleSwap::initPieces()
{
    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces();
}

void GameSimpleSwap::setAnimationToPieces()
{
    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces) {
        piece->setAnimation(std::make_shared<Animation::WarpMove>(0));
        piece->setEffect(frame);
    }
}

void GameSimpleSwap::setGraduallyFrame()
{
    Q_ASSERT(!pieces.isEmpty());

//    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(
//                              5, QColor(255, 255, 96, 0), QColor(255, 255, 96, 0), QColor(255, 255, 96, 192), QColor(255, 255, 96, 160), 180, true);

    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(
                              8, QColor(255, 128, 64, 224), QColor(255, 255, 64, 0), QColor(255, 255, 64, 224), QColor(255, 128, 64, 0), 240, true);

    auto compositeEffect = std::make_shared<Effect::CompositeEffect>();

    auto piece = getPiece(swapTargetPos.selectedPosition());

    compositeEffect->addEffect(piece->effect());
    compositeEffect->addEffect(graduallyFrame);

    piece->setEffect(compositeEffect);

    addChangedPieces({piece});
}

} // Fifteen
