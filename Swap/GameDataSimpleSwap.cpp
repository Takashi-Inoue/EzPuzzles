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
#include "GameDataSimpleSwap.h"
#include "CommonPhase/PhaseShowFinalImage.h"
#include "CommonPhase/PhaseShuffle.h"
#include "CommonPhase/PhaseCleared.h"
#include "fifteen/FifteenSwapShuffler.h"
#include "fifteen/SimplePiecesFactory.h"
#include "fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSwapEnding.h"
#include "PhaseSimpleSwapGaming.h"
#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Effect/CompositeEffect.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "SaveDataSimpleSwap.h"
#include "EzPuzzles.h"

#include <QDebug>

GameDataSimpleSwap::GameDataSimpleSwap(const SourceImage &img, const UniquePosition &swapTargetPos, const QSize &xyCount) :
    sourceImg(img),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(xyCount, img.size()), pieces)),
    swapTargetPos(swapTargetPos),
    currentPhaseType(IPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

QString GameDataSimpleSwap::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::SimpleSlide);
}

PhasePointer GameDataSimpleSwap::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    if (phaseType == IPhase::PhaseReady) {
        initPieces();
        return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
    }

    if (phaseType == IPhase::PhasePreGame)
        return std::make_shared<PhaseShuffle>(std::make_shared<Fifteen::SwapShuffler>(pieces, board->boardInfo()), IPhase::PhaseGaming);

    if (phaseType == IPhase::PhaseGaming)
        return std::make_shared<PhaseSimpleSwapGaming>(board, swapTargetPos.selectedPosition(), IPhase::PhaseEnding, slideFrameCount);

    if (phaseType == IPhase::PhaseEnding)
        return std::make_shared<PhaseSimpleSwapEnding>(pieces, IPhase::PhaseCleared);

    if (phaseType == IPhase::PhaseCleared)
        return std::make_shared<PhaseCleared>(sourceImg, IPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    currentPhaseType = IPhase::PhaseReady;

    return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
}

PhasePointer GameDataSimpleSwap::createCurrentPhase()
{
    return createPhase(currentPhaseType);
}

const SourceImage &GameDataSimpleSwap::sourceImage() const
{
    return sourceImg;
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

    savedata.gameTypeName     = gameName();
    savedata.boardSize        = board->boardInfo()->boardSize();
    savedata.swapTargetPos    = swapTargetPos;
    savedata.sourceImg        = sourceImg;
    savedata.currentPhaseType = currentPhaseType;

    for (const auto &piece : pieces)
        savedata.defaultPositions << piece->pos().defaultPos();

    return savedata.save();
}

bool GameDataSimpleSwap::load(const QString &fileName)
{
    SaveDataSimpleSwap savedata(fileName);

    if (!savedata.load() || savedata.gameName() != gameName())
        return false;

    swapTargetPos    = savedata.swapTargetPos;
    sourceImg        = savedata.sourceImg;
    currentPhaseType = savedata.currentPhaseType;

    board = std::make_shared<Board>(std::make_shared<BoardInformation>(savedata.boardSize, sourceImg.size()), pieces);

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces(savedata.defaultPositions);

    initPieces();

    return true;
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

//    for (auto &piece : pieces)
//        piece->setAnimation(std::make_shared<Animation::LineMove>(slideFrameCount, false));
}

void GameDataSimpleSwap::setEffectToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces)
        piece->setEffect(frame);

    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(
                              8, QColor(255, 128, 64, 224), QColor(255, 255, 64, 0), QColor(255, 255, 64, 224), QColor(255, 128, 64, 0), 240, true);

    auto compositeEffect = std::make_shared<Effect::CompositeEffect>();

    auto piece = getPiece(swapTargetPos.selectedPosition());

    compositeEffect->addEffect(piece->effect());
    compositeEffect->addEffect(graduallyFrame);

    piece->setEffect(compositeEffect);
}

Fifteen::PuzzlePiecePointer &GameDataSimpleSwap::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * board->boardInfo()->xCount() + pos.x()];
}
