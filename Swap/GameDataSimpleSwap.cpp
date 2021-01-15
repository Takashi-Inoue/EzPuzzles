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
#include "CommonPhase/PhaseCleared.h"
#include "Fifteen/FifteenSwapShuffler.h"
#include "Fifteen/SimplePiecesFactory.h"
#include "Fifteen/SlideBlankPiece.h"
#include "PhaseSimpleSwapEnding.h"
#include "PhaseSimpleSwapGaming.h"
#include "AnimationObject/Animation/AnimationWarpMove.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Transform/ChainedTransform.h"
#include "AnimationObject/Transform/TransformExpand.h"
#include "SaveDataSimpleSwap.h"
#include "Application.h"

#include <QDebug>

namespace Swap {

GameDataSimpleSwap::GameDataSimpleSwap(const SourceImage &img, const UniquePosition &swapTargetPos, const QSize &xyCount) :
    m_sourceImg(img),
    m_board(QSharedPointer<Fifteen::Board>::create(QSharedPointer<BoardInformation>::create(xyCount, img.size()), m_pieces)),
    m_swapTargetPos(swapTargetPos),
    m_currentPhaseType(AbstractPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSwap::GameDataSimpleSwap(const SaveDataSimpleSwap &loadedSaveData) :
    m_sourceImg(loadedSaveData.sourceImage()),
    m_board(QSharedPointer<Fifteen::Board>::create(loadedSaveData.boardInformation(), m_pieces)),
    m_swapTargetPos(loadedSaveData.specifiedPosition()),
    m_currentPhaseType(loadedSaveData.currentPhase())
{
    Q_ASSERT(loadedSaveData.isValid());

    m_pieces = Fifteen::SimplePiecesFactory(m_board->boardInfo(), m_sourceImg.pixmap())
            .createPieces(loadedSaveData.defaultPositions());

    initPieces();
}

GameDataPointer GameDataSimpleSwap::cloneAsNewGame() const
{
    return QSharedPointer<GameDataSimpleSwap>::create(
                m_sourceImg, m_swapTargetPos, m_board->boardInfo()->xyCount());
}

QString GameDataSimpleSwap::gameName() const
{
    return Application::gameName(Application::SimpleSwap);
}

PhasePointer GameDataSimpleSwap::createPhase(AbstractPhase::PhaseType phaseType)
{
    m_currentPhaseType = phaseType;

    if (phaseType == AbstractPhase::PhaseReady) {
        initPieces();
        Fifteen::SwapShuffler(m_pieces, m_board->boardInfo()).exec();

        return QSharedPointer<PhaseShowFinalImage>::create(m_sourceImg, AbstractPhase::PhaseGaming);
    }

    if (phaseType == AbstractPhase::PhaseGaming)
        return QSharedPointer<PhaseSimpleSwapGaming>::create(m_board, m_swapTargetPos.selectedPosition(), AbstractPhase::PhaseEnding, warpWaitCount * 2);

    if (phaseType == AbstractPhase::PhaseEnding)
        return QSharedPointer<PhaseSimpleSwapEnding>::create(m_board, AbstractPhase::PhaseCleared);

    if (phaseType == AbstractPhase::PhaseCleared)
        return QSharedPointer<PhaseCleared>::create(m_sourceImg, AbstractPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    m_currentPhaseType = AbstractPhase::PhaseReady;

    return QSharedPointer<PhaseShowFinalImage>::create(m_sourceImg, AbstractPhase::PhaseGaming);
}

AbstractPhase::PhaseType GameDataSimpleSwap::currentPhase() const
{
    return m_currentPhaseType;
}

const SourceImage &GameDataSimpleSwap::sourceImage() const
{
    return m_sourceImg;
}

FinalImagePointer GameDataSimpleSwap::finalImage() const
{
    return FinalImagePointer::create(m_sourceImg.pixmap());
}

BoardInfoPointer GameDataSimpleSwap::boardInfo() const
{
    return m_board->boardInfo();
}

bool GameDataSimpleSwap::save(QStringView fileName) const
{
    if (m_currentPhaseType == AbstractPhase::PhaseCleared)
        return m_sourceImg.saveImage();

    QList<QPoint> defaultPositions;

    for (const auto &piece : m_pieces)
        defaultPositions << piece->pos().defaultPos();

    SaveDataSimpleSwap savedata(fileName, m_board->boardInfo()->xyCount(), m_swapTargetPos, m_sourceImg
                              , m_currentPhaseType, defaultPositions);

    return savedata.write();
}

void GameDataSimpleSwap::initPieces()
{
    if (m_pieces.isEmpty())
        m_pieces = Fifteen::SimplePiecesFactory(m_board->boardInfo(), m_sourceImg.pixmap()).createPieces();

    setAnimationToPieces();
    setEffectToPieces();
}

void GameDataSimpleSwap::setAnimationToPieces()
{
    Q_ASSERT(!m_pieces.isEmpty());

    for (auto &piece : m_pieces) {
        auto chainTransform = QSharedPointer<Transform::ChainedTransform>::create();
        chainTransform->addTransform(QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalToCenter, warpWaitCount));
        chainTransform->addTransform(QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalFromCenter, warpWaitCount));

        piece->setAnimation(QSharedPointer<Animation::WarpMove>::create(warpWaitCount));
        piece->setTransform(chainTransform);
    }
}

void GameDataSimpleSwap::setEffectToPieces()
{
    Q_ASSERT(!m_pieces.isEmpty());

    auto frame = QSharedPointer<Effect::SimpleFrame>::create(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : m_pieces)
        piece->setEffect(frame);
}

} // Swap
