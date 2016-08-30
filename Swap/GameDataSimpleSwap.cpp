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
#include "GameDataSimpleSwap.h"
#include "CommonPhase/PhaseShowFinalImage.h"
#include "CommonPhase/PhaseShuffle.h"
#include "CommonPhase/PhaseCleared.h"
#include "fifteen/FifteenSwapShuffler.h"
#include "fifteen/SimplePiecesFactory.h"
#include "fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSwapEnding.h"
#include "PhaseSimpleSwapGaming.h"
#include "AnimationObject/Animation/AnimationWarpMove.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Transform/ChainedTransform.h"
#include "AnimationObject/Transform/TransformExpand.h"
#include "SaveDataSimpleSwap.h"
#include "EzPuzzles.h"

#include <QDebug>

namespace Swap {

GameDataSimpleSwap::GameDataSimpleSwap(const SourceImage &img, const UniquePosition &swapTargetPos, const QSize &xyCount) :
    rwlock(std::make_shared<QReadWriteLock>()),
    sourceImg(img),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(xyCount, img.size()), pieces, rwlock)),
    swapTargetPos(swapTargetPos),
    currentPhaseType(IPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSwap::GameDataSimpleSwap(const SaveDataSimpleSwap &loadedSaveData) :
    rwlock(std::make_shared<QReadWriteLock>()),
    sourceImg(loadedSaveData.sourceImg),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(loadedSaveData.boardSize, sourceImg.size()), pieces, rwlock)),
    swapTargetPos(loadedSaveData.swapTargetPos),
    currentPhaseType(loadedSaveData.currentPhaseType)
{
    Q_ASSERT(loadedSaveData.isValid());

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces(loadedSaveData.defaultPositions);

    initPieces();
}

GameDataPointer GameDataSimpleSwap::cloneAsNewGame() const
{
    return std::make_shared<GameDataSimpleSwap>(sourceImg, swapTargetPos, board->boardInfo()->boardSize());
}

QString GameDataSimpleSwap::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::SimpleSwap);
}

PhasePointer GameDataSimpleSwap::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    if (phaseType == IPhase::PhaseReady) {
        initPieces();
        return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
    }

    if (phaseType == IPhase::PhasePreGame)
        return std::make_shared<PhaseShuffle>(board, new Fifteen::SwapShuffler(pieces, board->boardInfo(), rwlock), IPhase::PhaseGaming);

    if (phaseType == IPhase::PhaseGaming)
        return std::make_shared<PhaseSimpleSwapGaming>(board, pieces, swapTargetPos.selectedPosition(), IPhase::PhaseEnding, warpWaitCount * 2);

    if (phaseType == IPhase::PhaseEnding)
        return std::make_shared<PhaseSimpleSwapEnding>(board->boardInfo(), pieces, IPhase::PhaseCleared);

    if (phaseType == IPhase::PhaseCleared)
        return std::make_shared<PhaseCleared>(sourceImg, IPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    currentPhaseType = IPhase::PhaseReady;

    return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
}

IPhase::PhaseType GameDataSimpleSwap::currentPhase() const
{
    return currentPhaseType;
}

const SourceImage &GameDataSimpleSwap::sourceImage() const
{
    return sourceImg;
}

FinalImagePointer GameDataSimpleSwap::finalImage() const
{
    return std::make_shared<FinalImage>(sourceImg.pixmap);
}

BoardInfoPointer GameDataSimpleSwap::boardInfo() const
{
    return board->boardInfo();
}

bool GameDataSimpleSwap::save(const QString &fileName) const
{
    if (currentPhaseType == IPhase::PhaseCleared)
        return sourceImg.saveImage();

    SaveDataSimpleSwap savedata(fileName);

    savedata.gameName         = gameName();
    savedata.boardSize        = board->boardInfo()->boardSize();
    savedata.swapTargetPos    = swapTargetPos;
    savedata.sourceImg        = sourceImg;
    savedata.currentPhaseType = currentPhaseType;

    for (const auto &piece : pieces)
        savedata.defaultPositions << piece->pos().defaultPos();

    return savedata.save();
}

void GameDataSimpleSwap::initPieces()
{
    if (pieces.isEmpty())
        pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces();

    setAnimationToPieces();
    setEffectToPieces();
}

void GameDataSimpleSwap::setAnimationToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    for (auto &piece : pieces) {
        auto chainTransform = std::make_shared<Transform::ChainedTransform>();
        chainTransform->addTransform(std::make_shared<Transform::Expand>(Transform::Expand::HorizontalToCenter, warpWaitCount));
        chainTransform->addTransform(std::make_shared<Transform::Expand>(Transform::Expand::HorizontalFromCenter, warpWaitCount));

        piece->setAnimation(std::make_shared<Animation::WarpMove>(warpWaitCount));
        piece->setTransform(chainTransform);
    }
}

void GameDataSimpleSwap::setEffectToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces)
        piece->setEffect(frame);
}

} // Swap
