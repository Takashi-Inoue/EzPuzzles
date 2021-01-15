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
#include "CommonPhase/PhaseCleared.h"
#include "Fifteen/FifteenSlideShuffler.h"
#include "Fifteen/SimplePiecesFactory.h"
#include "Fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSlideEnding.h"
#include "PhaseSimpleSlideGaming.h"
#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "SaveDataSimpleSlide.h"
#include "Application.h"

#include <QDebug>

namespace Slide {

GameDataSimpleSlide::GameDataSimpleSlide(const SourceImage &img, const UniquePosition &defaultBlankPos, const QSize &xyCount) :
    sourceImg(img),
    board(QSharedPointer<Fifteen::Board>::create(QSharedPointer<BoardInformation>::create(xyCount, img.size()), pieces)),
    defaultBlankPos(defaultBlankPos),
    currentBlankPos(defaultBlankPos.selectedPosition()),
    currentPhaseType(AbstractPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSlide::GameDataSimpleSlide(const SaveDataSimpleSlide &loadedSavedata) :
    sourceImg(loadedSavedata.sourceImage()),
    board(QSharedPointer<Fifteen::Board>::create(loadedSavedata.boardInformation(), pieces)),
    defaultBlankPos(loadedSavedata.specifiedPosition()),
    currentBlankPos(loadedSavedata.currentBlankPosition()),
    currentPhaseType(loadedSavedata.currentPhase())
{
    Q_ASSERT(loadedSavedata.isValid());

    pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap())
            .createPieces(loadedSavedata.defaultPositions());

    initPieces();
}

GameDataPointer GameDataSimpleSlide::cloneAsNewGame() const
{
    return QSharedPointer<GameDataSimpleSlide>::create(
                sourceImg, defaultBlankPos, board->boardInfo()->xyCount());
}

QString GameDataSimpleSlide::gameName() const
{
    return Application::gameName(Application::SimpleSlide);
}

PhasePointer GameDataSimpleSlide::createPhase(AbstractPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    if (phaseType == AbstractPhase::PhaseReady) {
        if (defaultBlankPos.isRandom()) {
            defaultBlankPos.randomSelect(board->boardInfo()->xyCount());
            currentBlankPos = defaultBlankPos.selectedPosition();
        }

        initPieces();
        Fifteen::SlideShuffler(pieces, board->boardInfo(), currentBlankPos).exec();

        return QSharedPointer<PhaseShowFinalImage>::create(sourceImg, AbstractPhase::PhaseGaming);
    }

    if (phaseType == AbstractPhase::PhaseGaming)
        return QSharedPointer<PhaseSimpleSlideGaming>::create(board, currentBlankPos, defaultBlankPos.selectedPosition(), AbstractPhase::PhaseEnding, slideFrameCount);

    if (phaseType == AbstractPhase::PhaseEnding)
        return QSharedPointer<PhaseSimpleSlideEnding>::create(board->boardInfo(), pieces, sourceImg.pixmap(), currentBlankPos, AbstractPhase::PhaseCleared);

    if (phaseType == AbstractPhase::PhaseCleared) {
        getPiece(currentBlankPos) = finalPiece;
        finalPiece.reset();

        return QSharedPointer<PhaseCleared>::create(sourceImg, AbstractPhase::PhaseReady);
    }

    qDebug() << QStringLiteral("no such phase type") << phaseType;

    currentPhaseType = AbstractPhase::PhaseReady;

    return QSharedPointer<PhaseShowFinalImage>::create(sourceImg, AbstractPhase::PhaseGaming);
}

AbstractPhase::PhaseType GameDataSimpleSlide::currentPhase() const
{
    return currentPhaseType;
}

const SourceImage &GameDataSimpleSlide::sourceImage() const
{
    return sourceImg;
}

FinalImagePointer GameDataSimpleSlide::finalImage() const
{
    return FinalImagePointer::create(sourceImg.pixmap());
}

BoardInfoPointer GameDataSimpleSlide::boardInfo() const
{
    return board->boardInfo();
}

bool GameDataSimpleSlide::save(QStringView fileName) const
{
    if (currentPhaseType == AbstractPhase::PhaseCleared)
        return sourceImg.saveImage();

    QList<QPoint> defaultPositions;

    for (const auto &piece : pieces)
        defaultPositions << piece->pos().defaultPos();

    SaveDataSimpleSlide savedata(fileName, board->boardInfo()->xyCount(), defaultBlankPos
                               , sourceImg, currentPhaseType, defaultPositions, currentBlankPos);

    return savedata.write();
}

void GameDataSimpleSlide::initPieces()
{
    if (pieces.isEmpty())
        pieces = Fifteen::SimplePiecesFactory(board->boardInfo(), sourceImg.pixmap()).createPieces();

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

    blankPiece = QSharedPointer<Fifteen::SlideBlankPiece>::create(board->boardInfo(), defaultBlankPos.selectedPosition(), Qt::black, slideFrameCount);
    blankPiece->setPosWithoutAnimation(currentBlankPos);
}

void GameDataSimpleSlide::setSlideAnimationToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    for (auto &piece : pieces)
        piece->setAnimation(QSharedPointer<Animation::LineMove>::create(slideFrameCount, false));
}

void GameDataSimpleSlide::setEffectToPieces()
{
    Q_ASSERT(!pieces.isEmpty());

    auto frame = QSharedPointer<Effect::SimpleFrame>::create(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces)
        piece->setEffect(frame);

    auto graduallyFrame = QSharedPointer<Effect::GraduallyBlinkFrame>::create(15, QColor(0, 0, 0), QColor(0, 0, 0), QColor(64, 192, 224, 224), QColor(16, 64, 96, 224), 120, true);

    getPiece(currentBlankPos)->setEffect(graduallyFrame);
}

FifteenPiecePointer &GameDataSimpleSlide::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * board->boardInfo()->xCount() + pos.x()];
}

} // Slide
