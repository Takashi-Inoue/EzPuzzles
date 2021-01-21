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
#include "GameDataMineSweeper.h"
#include "CommonPhase/PhaseCleared.h"
#include "MinePiecesFactory.h"
#include "PhaseMineSweeperEnding.h"
#include "PhaseMineSweeperGaming.h"
#include "SaveDataMineSweeper.h"
#include "Application.h"

namespace MineSweeper {

GameDataMineSweeper::GameDataMineSweeper(const SourceImage &sourceImage, const QSize &countXY, int mineCount, bool isAutoLock) :
    sourceImg(sourceImage),
    boardInformation(QSharedPointer<BoardInformation>::create(countXY, sourceImage.size())),
    mineCount(mineCount),
    currentPhaseType(AbstractPhase::PhaseReady),
    m_isAutoLock(isAutoLock)
{
    // Initializing pieces is in createPhase
}

GameDataMineSweeper::GameDataMineSweeper(const SaveDataMineSweeper &loadedSavedata) :
    sourceImg(loadedSavedata.sourceImage()),
    boardInformation(loadedSavedata.boardInformation()),
    mineCount(loadedSavedata.mineCount()),
    currentPhaseType(loadedSavedata.currentPhase()),
    m_isAutoLock(loadedSavedata.isAutoLock())
{
    Q_ASSERT(loadedSavedata.isValid());

    PiecesFactory factory(sourceImg.pixmap(), boardInformation, mineCount);
    pieces = factory.toPieces(loadedSavedata.piecesAsIntList());

    mineField = MineFieldPointer::create(pieces, m_isAutoLock, mineCount
                                       , loadedSavedata.openedCount(), loadedSavedata.missedCount()
                                       , loadedSavedata.savers());
}

GameDataPointer GameDataMineSweeper::cloneAsNewGame() const
{
    return QSharedPointer<GameDataMineSweeper>::create(
                sourceImg, boardInformation->xyCount(), mineCount, m_isAutoLock);
}

QString GameDataMineSweeper::gameName() const
{
    return Application::gameName(Application::MineSweeper);
}

PhasePointer GameDataMineSweeper::createPhase(AbstractPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    switch (phaseType) {
    case AbstractPhase::PhaseReady: {
        pieces = PiecesFactory(sourceImg.pixmap(), boardInformation, mineCount).createPieces();
        mineField = MineFieldPointer::create(pieces, m_isAutoLock, mineCount);
    } // through to case IPhase::PhaseGaming
    [[fallthrough]];

    case AbstractPhase::PhaseGaming:
        currentPhaseType = AbstractPhase::PhaseGaming;
        return QSharedPointer<PhaseMineSweeperGaming>::create(mineField, AbstractPhase::PhaseEnding);

    case AbstractPhase::PhaseEnding:
        return QSharedPointer<PhaseMineSweeperEnding>::create(boardInformation, pieces, sourceImg, AbstractPhase::PhaseCleared);

    case AbstractPhase::PhaseCleared:
        return QSharedPointer<PhaseCleared>::create(sourceImg, AbstractPhase::PhaseReady);
    }

    return nullptr;
}

AbstractPhase::PhaseType GameDataMineSweeper::currentPhase() const
{
    return currentPhaseType;
}

const SourceImage &GameDataMineSweeper::sourceImage() const
{
    return sourceImg;
}

FinalImagePointer GameDataMineSweeper::finalImage() const
{
    return nullptr;
}

BoardInfoPointer GameDataMineSweeper::boardInfo() const
{
    return boardInformation;
}

bool GameDataMineSweeper::save(QStringView fileName) const
{
    SaveDataMineSweeper savedata(fileName, boardInformation->xyCount(), mineCount
                               , mineField->openedCount(), mineField->missedCount()
                               , m_isAutoLock, sourceImg, currentPhaseType
                               , PiecesFactory::toIntList(pieces), mineField->savers());

    return savedata.write();
}

double GameDataMineSweeper::openedRate() const
{
    return mineField->openedRate();
}

} // MineSweeper
