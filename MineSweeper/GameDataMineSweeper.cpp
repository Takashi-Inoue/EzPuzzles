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
    currentPhaseType(IPhase::PhaseReady)
{
    // pieces の初期化は createPhase で

    if (isAutoLock)
        mineLocker = std::make_shared<MineLocker>(pieces);
}

GameDataMineSweeper::GameDataMineSweeper(const SaveDataMineSweeper &loadedSavedata) :
    sourceImg(loadedSavedata.sourceImage()),
    boardInformation(loadedSavedata.boardInformation()),
    mineCount(loadedSavedata.mineCount()),
    currentPhaseType(loadedSavedata.currentPhase())
{
    Q_ASSERT(loadedSavedata.isValid());

    PiecesFactory factory(sourceImg.pixmap(), boardInformation, mineCount, loadedSavedata.m_isAutoLock);
    pieces = factory.toPieces(loadedSavedata.m_pieces);

    if (loadedSavedata.m_isAutoLock) {
        mineLocker = std::make_shared<MineLocker>(pieces);
        mineLocker->addMinesPositions(factory.minesPositions());
    }

    mineField = MineFieldPointer::create(pieces, mineLocker, mineCount);
    mineField->load(loadedSavedata);

    finalImg = QSharedPointer<MineSweeperFinalImage>::create(
                   sourceImg.pixmap(), mineField, boardInformation);
}

GameDataPointer GameDataMineSweeper::cloneAsNewGame() const
{
    return QSharedPointer<GameDataMineSweeper>::create(sourceImg, boardInformation->xyCount(), mineCount, mineLocker != nullptr);
}

QString GameDataMineSweeper::gameName() const
{
    return Application::gameName(Application::MineSweeper);
}

PhasePointer GameDataMineSweeper::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    switch (phaseType) {
    case IPhase::PhaseReady: {
        PiecesFactory factory(sourceImg.pixmap(), boardInformation, mineCount, mineLocker != nullptr);
        pieces = factory.createPieces();

        if (mineLocker != nullptr)
            mineLocker->setMinesPositions(factory.minesPositions());

        mineField = MineFieldPointer::create(pieces, mineLocker, mineCount);

        finalImg = QSharedPointer<MineSweeperFinalImage>::create(
                       sourceImg.pixmap(), mineField, boardInformation);
    } // through to case IPhase::PhaseGaming
    [[fallthrough]];

    case IPhase::PhasePreGame:
    case IPhase::PhaseGaming:
        currentPhaseType = IPhase::PhaseGaming;
        return QSharedPointer<PhaseMineSweeperGaming>::create(mineField, pieces, IPhase::PhaseEnding);

    case IPhase::PhaseEnding:
        return QSharedPointer<PhaseMineSweeperEnding>::create(boardInformation, pieces, sourceImg, IPhase::PhaseCleared);

    case IPhase::PhaseCleared:
        return QSharedPointer<PhaseCleared>::create(sourceImg, IPhase::PhaseReady);
    }

    return nullptr;
}

IPhase::PhaseType GameDataMineSweeper::currentPhase() const
{
    return currentPhaseType;
}

const SourceImage &GameDataMineSweeper::sourceImage() const
{
    return sourceImg;
}

FinalImagePointer GameDataMineSweeper::finalImage() const
{
    return finalImg;
}

BoardInfoPointer GameDataMineSweeper::boardInfo() const
{
    return boardInformation;
}

bool GameDataMineSweeper::save(QStringView fileName) const
{
    SaveDataMineSweeper savedata(fileName, boardInformation->xyCount(), mineCount
                               , mineField->openedCount(), mineField->missedCount()
                               , mineLocker != nullptr, sourceImg, currentPhaseType
                               , PiecesFactory::toIntList(pieces));

    return savedata.write();
}

double GameDataMineSweeper::openedRate() const
{
    return mineField->openedRate();
}

QList<QPointF> GameDataMineSweeper::explodedCenters() const
{
    QList<QPointF> centers;

    for (const auto &piecePos : mineField->explodedPositions()) {
        const auto &rect = boardInformation->rectFromPiecePos(piecePos - QPoint(1, 1));

        centers << rect.center();
    }

    return centers;
}

} // MineSweeper
