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
    m_rwlock(std::make_shared<QReadWriteLock>()),
    m_sourceImg(img),
    m_board(std::make_shared<Board>(QSharedPointer<BoardInformation>::create(xyCount, img.size()), m_pieces, m_rwlock)),
    m_swapTargetPos(swapTargetPos),
    m_currentPhaseType(IPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSwap::GameDataSimpleSwap(const SaveDataSimpleSwap &loadedSaveData) :
    m_rwlock(std::make_shared<QReadWriteLock>()),
    m_sourceImg(loadedSaveData.sourceImage()),
    m_board(std::make_shared<Board>(loadedSaveData.boardInformation(), m_pieces, m_rwlock)),
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
    return QSharedPointer<GameDataSimpleSwap>::create(m_sourceImg, m_swapTargetPos, m_board->boardInfo()->countXY());
}

QString GameDataSimpleSwap::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::SimpleSwap);
}

PhasePointer GameDataSimpleSwap::createPhase(IPhase::PhaseType phaseType)
{
    m_currentPhaseType = phaseType;

    if (phaseType == IPhase::PhaseReady) {
        initPieces();
        return std::make_shared<PhaseShowFinalImage>(m_sourceImg, IPhase::PhasePreGame);
    }

    if (phaseType == IPhase::PhasePreGame)
        return std::make_shared<PhaseShuffle>(m_board, new Fifteen::SwapShuffler(m_pieces, m_board->boardInfo(), m_rwlock), IPhase::PhaseGaming);

    if (phaseType == IPhase::PhaseGaming)
        return std::make_shared<PhaseSimpleSwapGaming>(m_board, m_pieces, m_swapTargetPos.selectedPosition(), IPhase::PhaseEnding, warpWaitCount * 2);

    if (phaseType == IPhase::PhaseEnding)
        return std::make_shared<PhaseSimpleSwapEnding>(m_board->boardInfo(), m_pieces, IPhase::PhaseCleared);

    if (phaseType == IPhase::PhaseCleared)
        return std::make_shared<PhaseCleared>(m_sourceImg, IPhase::PhaseReady);

    qDebug() << "no such phase type" << phaseType;

    m_currentPhaseType = IPhase::PhaseReady;

    return std::make_shared<PhaseShowFinalImage>(m_sourceImg, IPhase::PhasePreGame);
}

IPhase::PhaseType GameDataSimpleSwap::currentPhase() const
{
    return m_currentPhaseType;
}

const SourceImage &GameDataSimpleSwap::sourceImage() const
{
    return m_sourceImg;
}

FinalImagePointer GameDataSimpleSwap::finalImage() const
{
    return std::make_shared<FinalImage>(m_sourceImg.pixmap());
}

BoardInfoPointer GameDataSimpleSwap::boardInfo() const
{
    return m_board->boardInfo();
}

bool GameDataSimpleSwap::save(const QString &fileName) const
{
    if (m_currentPhaseType == IPhase::PhaseCleared)
        return m_sourceImg.saveImage();

    QList<QPoint> defaultPositions;

    for (const auto &piece : m_pieces)
        defaultPositions << piece->pos().defaultPos();

    SaveDataSimpleSwap savedata(fileName, m_board->boardInfo()->countXY(), m_swapTargetPos, m_sourceImg
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
        auto chainTransform = std::make_shared<Transform::ChainedTransform>();
        chainTransform->addTransform(std::make_shared<Transform::Expand>(Transform::Expand::HorizontalToCenter, warpWaitCount));
        chainTransform->addTransform(std::make_shared<Transform::Expand>(Transform::Expand::HorizontalFromCenter, warpWaitCount));

        piece->setAnimation(std::make_shared<Animation::WarpMove>(warpWaitCount));
        piece->setTransform(chainTransform);
    }
}

void GameDataSimpleSwap::setEffectToPieces()
{
    Q_ASSERT(!m_pieces.isEmpty());

    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : m_pieces)
        piece->setEffect(frame);
}

} // Swap
