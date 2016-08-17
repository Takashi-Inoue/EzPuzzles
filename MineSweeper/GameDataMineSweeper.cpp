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
#include "GameDataMineSweeper.h"
#include "CommonPhase/PhaseCleared.h"
#include "mine/MinePiecesFactory.h"
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
    PiecesFactory factory(sourceImage.pixmap, boardInformation, mineCount, isAutoLock);
    pieces = factory.createPieces();

    if (isAutoLock) {
        mineLocker = std::make_shared<MineLocker>(pieces);
        mineLocker->addMinesPositions(factory.getMinesPositions());
    }

    mineField = std::make_shared<MineField>(pieces, mineLocker, mineCount);
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
}

QString GameDataMineSweeper::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::MineSweeper);
}

PhasePointer GameDataMineSweeper::createPhase(IPhase::PhaseType phaseType)
{
    currentPhaseType = phaseType;

    switch (phaseType) {
    case IPhase::PhaseReady:
    case IPhase::PhasePreGame:
    case IPhase::PhaseGaming:
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

QPixmap GameDataMineSweeper::finalImage() const
{
    return sourceImg.pixmap;
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
