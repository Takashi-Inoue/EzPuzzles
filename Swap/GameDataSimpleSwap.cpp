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

#include "Application.h"

#include "CommonPhase/PhaseCleared.h"
#include "PhaseSimpleSwapEnding.h"
#include "PhaseSimpleSwapGaming.h"

#include "SaveDataSimpleSwap.h"
#include "SwapGameBoard.h"
#include "SwapEndingBoard.h"

#include <QDebug>

namespace Swap {

GameDataSimpleSwap::GameDataSimpleSwap(const SourceImage &img, const UniquePosition &swapTargetPos, const QSize &xyCount) :
    m_sourceImg(img),
    m_board(QSharedPointer<GameBoard>::create(QSharedPointer<BoardInformation>::create(xyCount, img.size()))),
    m_swapTargetPos(swapTargetPos),
    m_currentPhaseType(AbstractPhase::PhaseReady)
{
    Q_ASSERT(!img.isNull());
}

GameDataSimpleSwap::GameDataSimpleSwap(const SaveDataSimpleSwap &loadedSaveData) :
    m_sourceImg(loadedSaveData.sourceImage()),
    m_board(QSharedPointer<GameBoard>::create(loadedSaveData.boardInformation())),
    m_swapTargetPos(loadedSaveData.specifiedPosition()),
    m_currentPhaseType(loadedSaveData.currentPhase())
{
    Q_ASSERT(loadedSaveData.isValid());

    m_board->initPieces(m_sourceImg.pixmap(), loadedSaveData.defaultPositions());
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

    switch (phaseType) {
    case AbstractPhase::PhaseReady:
        m_board->initPieces(m_sourceImg.pixmap());
        m_board->shufflePieces();

        if (m_swapTargetPos.isRandom())
            m_swapTargetPos.randomSelect(m_board->boardInfo()->xyCount());

        m_currentPhaseType = AbstractPhase::PhaseGaming;

        [[fallthrough]];

    case AbstractPhase::PhaseGaming:
        return QSharedPointer<PhaseSimpleSwapGaming>::create(m_board, m_swapTargetPos.selectedPosition(), AbstractPhase::PhaseEnding);

    case AbstractPhase::PhaseEnding: {
        auto board = QSharedPointer<EndingBoard>::create(m_board->boardInfo(), m_sourceImg.pixmap());
        return QSharedPointer<PhaseSimpleSwapEnding>::create(board, AbstractPhase::PhaseCleared);
    }

    case AbstractPhase::PhaseCleared:
        return QSharedPointer<PhaseCleared>::create(m_sourceImg, AbstractPhase::PhaseReady);
    }
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

    SaveDataSimpleSwap savedata(fileName, m_board->boardInfo()->xyCount(), m_swapTargetPos, m_sourceImg
                              , m_currentPhaseType, m_board->defaultPiecesPos());

    return savedata.write();
}

} // Swap
