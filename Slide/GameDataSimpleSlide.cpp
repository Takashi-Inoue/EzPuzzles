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
    rwlock(std::make_shared<QReadWriteLock>()),
    sourceImg(img),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(xyCount, img.size()), pieces, rwlock)),
    defaultBlankPos(defaultBlankPos),
    currentBlankPos(defaultBlankPos.selectedPosition()),
    currentPhaseType(IPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSlide::GameDataSimpleSlide(const SaveDataSimpleSlide &loadedSavedata) :
    rwlock(std::make_shared<QReadWriteLock>()),
    sourceImg(loadedSavedata.sourceImg),
    board(std::make_shared<Board>(std::make_shared<BoardInformation>(loadedSavedata.boardSize, sourceImg.size()), pieces, rwlock)),
    defaultBlankPos(loadedSavedata.defaultBlankPos),
    currentBlankPos(loadedSavedata.currentBlankPos),
    currentPhaseType(loadedSavedata.currentPhaseType)
{
    Q_ASSERT(loadedSavedata.isValid());

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces(loadedSavedata.defaultPositions);

    initPieces();
}

GameDataPointer GameDataSimpleSlide::cloneAsNewGame() const
{
    return std::make_shared<GameDataSimpleSlide>(sourceImg, defaultBlankPos, board->boardInfo()->boardSize());
}

QString GameDataSimpleSlide::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::SimpleSlide);
}

PhasePointer GameDataSimpleSlide::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    if (phaseType == IPhase::PhaseReady) {
        initPieces();
        return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
    }

    if (phaseType == IPhase::PhasePreGame)
        return std::make_shared<PhaseShuffle>(board, new Fifteen::SlideShuffler(pieces, board->boardInfo(), currentBlankPos, rwlock), IPhase::PhaseGaming);

    if (phaseType == IPhase::PhaseGaming)
        return std::make_shared<PhaseSimpleSlideGaming>(board, currentBlankPos, IPhase::PhaseEnding, slideFrameCount);

    if (phaseType == IPhase::PhaseEnding)
        return std::make_shared<PhaseSimpleSlideEnding>(pieces, finalPiece, currentBlankPos.y() * board->boardInfo()->xCount() + currentBlankPos.x(), IPhase::PhaseCleared);

    if (phaseType == IPhase::PhaseCleared)
        return std::make_shared<PhaseCleared>(sourceImg, IPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    currentPhaseType = IPhase::PhaseReady;

    return std::make_shared<PhaseShowFinalImage>(sourceImg, IPhase::PhasePreGame);
}

IPhase::PhaseType GameDataSimpleSlide::currentPhase() const
{
    return currentPhaseType;
}

const SourceImage &GameDataSimpleSlide::sourceImage() const
{
    return sourceImg;
}

FinalImagePointer GameDataSimpleSlide::finalImage() const
{
    return std::make_shared<FinalImage>(sourceImg.pixmap);
}

BoardInfoPointer GameDataSimpleSlide::boardInfo() const
{
    return board->boardInfo();
}

bool GameDataSimpleSlide::save(const QString &fileName) const
{
    if (currentPhaseType == IPhase::PhaseCleared)
        return sourceImg.saveImage();

    SaveDataSimpleSlide savedata(fileName);

    savedata.gameName         = gameName();
    savedata.boardSize        = board->boardInfo()->boardSize();
    savedata.defaultBlankPos  = defaultBlankPos;
    savedata.currentBlankPos  = currentBlankPos;
    savedata.sourceImg        = sourceImg;
    savedata.currentPhaseType = currentPhaseType;

    for (const auto &piece : pieces)
        savedata.defaultPositions << piece->pos().defaultPos();

    return savedata.save();
}

void GameDataSimpleSlide::initPieces()
{
    if (pieces.isEmpty())
        pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap).createPieces();

    createBlankPiece();
    setSlideAnimationToPieces();
    setEffectToPieces();
}

void GameDataSimpleSlide::createBlankPiece()
{
    Q_ASSERT(!pieces.isEmpty());

    if (finalPiece == nullptr)
        finalPiece = getPiece(currentBlankPos);

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

    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(15, QColor(0, 0, 0), QColor(0, 0, 0), QColor(64, 192, 224, 224), QColor(16, 64, 96, 224), 120, true);

    getPiece(currentBlankPos)->setEffect(graduallyFrame);
}

Fifteen::PuzzlePiecePointer &GameDataSimpleSlide::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * board->boardInfo()->xCount() + pos.x()];
}
