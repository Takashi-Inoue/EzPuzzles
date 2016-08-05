﻿/*
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
#include "GameDataSimpleSlide.h"
#include "CommonPhase/PhaseShowFinalImage.h"
#include "CommonPhase/PhaseShuffle.h"
#include "CommonPhase/PhaseCleared.h"
#include "fifteen/FifteenSlideShuffler.h"
#include "fifteen/SimplePiecesFactory.h"
#include "fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSlideEnding.h"
#include "PhaseSimpleSlideGaming.h"
#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "SaveDataSimpleSlide.h"
#include "EzPuzzles.h"

#include <QDebug>

GameDataSimpleSlide::GameDataSimpleSlide(const SourceImage &img, const UniquePosition &defaultBlankPos, const QSize &xyCount) :
    sourceImg(img),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(xyCount, img.size()), pieces)),
    defaultBlankPos(defaultBlankPos),
    currentBlankPos(defaultBlankPos.selectedPosition()),
    currentPhaseType(IPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), img.pixmap).createPieces();

    createBlankPiece();
    setSlideAnimationToPieces();
    setEffectToPieces();
}

QString GameDataSimpleSlide::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::SimpleSlide);
}

PhasePointer GameDataSimpleSlide::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    if (phaseType == IPhase::PhaseReady)
        return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);

    if (phaseType == IPhase::PhasePreGame)
        return std::make_shared<PhaseShuffle>(std::make_shared<Fifteen::SlideShuffler>(pieces, board->boardInfo(), currentBlankPos), IPhase::PhaseGaming);

    if (phaseType == IPhase::PhaseGaming)
        return std::make_shared<PhaseSimpleSlideGaming>(board, currentBlankPos, IPhase::PhaseEnding, slideFrameCount);

    if (phaseType == IPhase::PhaseEnding)
        return std::make_shared<PhaseSimpleSlideEnding>(pieces, IPhase::PhaseCleared);

    if (phaseType == IPhase::PhaseCleared)
        return std::make_shared<PhaseCleared>(sourceImg, IPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    currentPhaseType = IPhase::PhaseReady;

    return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
}

PhasePointer GameDataSimpleSlide::createCurrentPhase()
{
    return createPhase(currentPhaseType);
}

const SourceImage &GameDataSimpleSlide::sourceImage() const
{
    return sourceImg;
}

BoardInfoPointer GameDataSimpleSlide::boardInfo() const
{
    return board->boardInfo();
}

bool GameDataSimpleSlide::save(const QString &fileName) const
{
    SaveDataSimpleSlide savedata(fileName);

    savedata.gameTypeName     = gameName();
    savedata.boardSize        = board->boardInfo()->boardSize();
    savedata.defaultBlankPos  = defaultBlankPos;
    savedata.currentBlankPos  = currentBlankPos;
    savedata.sourceImg        = sourceImg;
    savedata.currentPhaseType = currentPhaseType;

    for (const auto &piece : pieces)
        savedata.defaultPositions << piece->pos().defaultPos();

    return savedata.save();
}

bool GameDataSimpleSlide::load(const QString &fileName)
{
    SaveDataSimpleSlide savedata(fileName);

    if (!savedata.load() || savedata.gameName() != gameName())
        return false;

    defaultBlankPos  = savedata.defaultBlankPos;
    currentBlankPos  = savedata.currentBlankPos;
    sourceImg        = savedata.sourceImg;
    currentPhaseType = savedata.currentPhaseType;

    board = std::make_shared<Board>(std::make_shared<BoardInformation>(savedata.boardSize, sourceImg.size()), pieces);

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces(savedata.defaultPositions);

    createBlankPiece();
    setSlideAnimationToPieces();
    setEffectToPieces();

    return true;
}

void GameDataSimpleSlide::createBlankPiece()
{
    auto &blankPiece = getPiece(currentBlankPos);

    blankPiece = std::make_shared<Fifteen::SlideBlankPiece>(board->boardInfo(), defaultBlankPos.selectedPosition(), Qt::black, slideFrameCount);
    blankPiece->setPosWithoutAnimation(currentBlankPos);
}

void GameDataSimpleSlide::setSlideAnimationToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    for (auto &piece : pieces)
        piece->setAnimation(std::make_shared<Animation::LineMove>(slideFrameCount, false));
}

void GameDataSimpleSlide::setEffectToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces)
        piece->setEffect(frame);

    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(4, QColor(0, 0, 0), QColor(0, 0, 0), QColor(64, 192, 224, 224), QColor(16, 64, 96, 224), 120, true);

    getPiece(currentBlankPos)->setEffect(graduallyFrame);
}

Fifteen::PuzzlePiecePointer &GameDataSimpleSlide::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * board->boardInfo()->xCount() + pos.x()];
}
