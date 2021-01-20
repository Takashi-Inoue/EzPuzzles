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
#ifndef GAMEDATASIMPLESLIDE_H
#define GAMEDATASIMPLESLIDE_H

#include "IGameData.h"
#include "Fifteen/AbstractGameBoard.h"
#include "UniquePosition.h"

#include <QReadWriteLock>

namespace Slide {

class SaveDataSimpleSlide;

class GameDataSimpleSlide : public IGameData
{
public:
    GameDataSimpleSlide(const SourceImage &img, const UniquePosition &defaultBlankPos, const QSize &xyCount);
    GameDataSimpleSlide(const SaveDataSimpleSlide &loadedSavedata);

    // IGameData
    GameDataPointer cloneAsNewGame() const override;
    QString gameName() const override;
    PhasePointer createPhase(AbstractPhase::PhaseType) override;
    AbstractPhase::PhaseType currentPhase() const override;
    const SourceImage &sourceImage() const override;
    FinalImagePointer finalImage() const override;
    BoardInfoPointer boardInfo() const override;

    bool save(QStringView fileName) const override;

protected:
    void createBlankPiece();

    SourceImage m_sourceImg;
    GameBoardPtr m_board;
    UniquePosition m_defaultBlankPos;
    QPoint m_currentBlankPos;
    AbstractPhase::PhaseType m_currentPhaseType;
};

} // Slide

#endif // GAMEDATASIMPLESLIDE_H
