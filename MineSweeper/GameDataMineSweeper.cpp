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
#include "mine/MinePiecesFactory.h"
#include "PhaseMineSweeperGaming.h"
#include "EzPuzzles.h"

namespace MineSweeper {

GameDataMineSweeper::GameDataMineSweeper(const SourceImage &sourceImage, const QSize &xyCount, int mineCount, bool isAutoLock) :
    sourceImg(sourceImage),
    boardInformation(std::make_shared<BoardInformation>(xyCount, sourceImage.size())),
    mineCount(mineCount)
{
    PiecesFactory factory(sourceImage.pixmap, xyCount, mineCount, isAutoLock);
    pieces = factory.createPieces();

    if (isAutoLock) {
        mineLocker = std::make_shared<MineLocker>(pieces);
        mineLocker->addMinesPositions(factory.getMinesPositions());
    }
}

QString GameDataMineSweeper::gameName() const
{
    return EzPuzzles::gameName(EzPuzzles::MineSweeper);
}

PhasePointer GameDataMineSweeper::createPhase(IPhase::PhaseType phaseType)
{
    switch (phaseType) {
    case IPhase::PhaseReady:
    case IPhase::PhasePreGame:
    case IPhase::PhaseGaming:
        return currentPhase = std::make_shared<PhaseMineSweeperGaming>(pieces, mineLocker, IPhase::PhaseEnding);

    case IPhase::PhaseEnding:
        break;

    case IPhase::PhaseCleared:
        break;
    }

    return nullptr;
}

PhasePointer GameDataMineSweeper::createCurrentPhase()
{
    return currentPhase;
}

const SourceImage &GameDataMineSweeper::sourceImage() const
{
    return sourceImg;
}

BoardInfoPointer GameDataMineSweeper::boardInfo() const
{
    return boardInformation;
}

bool GameDataMineSweeper::save(const QString &fileName) const
{
    return true;
}

bool GameDataMineSweeper::load(const QString &fileName)
{
    return true;
}

} // MineSweeper
