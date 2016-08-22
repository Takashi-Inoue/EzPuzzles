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
#include "PhaseMineSweeperGaming.h"
#include "SaveDataMineSweeper.h"
#include "EzPuzzles.h"

namespace MineSweeper {

GameDataMineSweeper::GameDataMineSweeper(const SourceImage &sourceImage, const QSize &xyCount, int mineCount, bool isAutoLock) :
    sourceImg(sourceImage),
    boardInformation(std::make_shared<BoardInformation>(xyCount, sourceImage.size())),
    mineCount(mineCount),
    currentPhaseType(IPhase::PhaseReady)
{
    // pieces の初期化は createPhase で

    if (isAutoLock)
        mineLocker = std::make_shared<MineLocker>(pieces);
}

GameDataMineSweeper::GameDataMineSweeper(const SaveDataMineSweeper &loadedSavedata) :
    sourceImg(loadedSavedata.sourceImg),
    boardInformation(std::make_shared<BoardInformation>(loadedSavedata.xyCount, sourceImg.size())),
    mineCount(loadedSavedata.mineCount),
    currentPhaseType(loadedSavedata.currentPhaseType)
{
    Q_ASSERT(loadedSavedata.isValid());

    PiecesFactory factory(sourceImg.pixmap, boardInformation, mineCount, loadedSavedata.isAutoLock);
    pieces = factory.toPieces(loadedSavedata.pieces);

    if (loadedSavedata.isAutoLock) {
        mineLocker = std::make_shared<MineLocker>(pieces);
        mineLocker->addMinesPositions(factory.getMinesPositions());
    }

    mineField = std::make_shared<MineField>(pieces, mineLocker, mineCount);
    mineField->load(loadedSavedata);

    finalImg = std::make_shared<MineSweeperFinalImage>(sourceImg.pixmap, mineField, boardInformation);
}

GameDataPointer GameDataMineSweeper::cloneAsNewGame() const
{
    return std::make_shared<GameDataMineSweeper>(sourceImg, boardInformation->boardSize(), mineCount, mineLocker != nullptr);
}

QString GameDataMineSweeper::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::MineSweeper);
}

PhasePointer GameDataMineSweeper::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    switch (phaseType) {
    case IPhase::PhaseReady: {
        PiecesFactory factory(sourceImg.pixmap, boardInformation, mineCount, mineLocker != nullptr);
        pieces = factory.createPieces();

        if (mineLocker != nullptr)
            mineLocker->setMinesPositions(factory.getMinesPositions());

        mineField = std::make_shared<MineField>(pieces, mineLocker, mineCount);

        finalImg = std::make_shared<MineSweeperFinalImage>(sourceImg.pixmap, mineField, boardInformation);

        currentPhaseType = IPhase::PhaseGaming;

        return std::make_shared<PhaseMineSweeperGaming>(mineField, pieces, IPhase::PhaseCleared);
    }

    case IPhase::PhasePreGame:
    case IPhase::PhaseGaming:
        currentPhaseType = IPhase::PhaseGaming;
        return std::make_shared<PhaseMineSweeperGaming>(mineField, pieces, IPhase::PhaseCleared);

    case IPhase::PhaseEnding:
        break;

    case IPhase::PhaseCleared:
        return std::make_shared<PhaseCleared>(sourceImg, IPhase::PhaseReady);
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

bool GameDataMineSweeper::save(const QString &fileName) const
{
    SaveDataMineSweeper savedata(fileName);

    savedata.xyCount = boardInformation->boardSize();
    savedata.mineCount = mineCount;
    savedata.isAutoLock = (mineLocker != nullptr);
    savedata.sourceImg = sourceImg;
    savedata.currentPhaseType = currentPhaseType;
    savedata.pieces = PiecesFactory::toIntList(pieces);

    mineField->save(savedata);

    return savedata.save();
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
