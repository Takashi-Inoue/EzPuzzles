﻿/*
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
#include "Board.h"
#include "UniquePosition.h"

#include <QReadWriteLock>

class SaveDataSimpleSlide;

class GameDataSimpleSlide : public IGameData
{
public:
    GameDataSimpleSlide(const SourceImage &img, const UniquePosition &defaultBlankPos, const QSize &xyCount);
    GameDataSimpleSlide(const SaveDataSimpleSlide &loadedSavedata);
    ~GameDataSimpleSlide() = default;

    // IGameData
    GameDataPointer cloneAsNewGame() const override;
    QString gameName() const override;
    PhasePointer createPhase(IPhase::PhaseType) override;
    IPhase::PhaseType currentPhase() const override;
    const SourceImage &sourceImage() const override;
    FinalImagePointer finalImage() const override;
    BoardInfoPointer boardInfo() const override;

    bool save(const QString &fileName) const override;

protected:
    void initPieces();
    void createBlankPiece();
    void setSlideAnimationToPieces();
    void setEffectToPieces();
    Fifteen::PuzzlePiecePointer &getPiece(const QPoint &pos);

    static const unsigned char slideFrameCount = 20;

    std::shared_ptr<QReadWriteLock> rwlock;

    QList<Fifteen::PuzzlePiecePointer> pieces;
    Fifteen::PuzzlePiecePointer finalPiece;
    SourceImage sourceImg;
    BoardPointer board;
    UniquePosition defaultBlankPos;
    QPoint currentBlankPos;
    IPhase::PhaseType currentPhaseType;
};

#endif // GAMEDATASIMPLESLIDE_H