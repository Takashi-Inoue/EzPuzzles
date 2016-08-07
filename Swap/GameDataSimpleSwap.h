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
#ifndef GAMEDATASIMPLESWAP_H
#define GAMEDATASIMPLESWAP_H

#include "IGameData.h"
#include "Board.h"
#include "UniquePosition.h"

class GameDataSimpleSwap : public IGameData
{
public:
    GameDataSimpleSwap(const SourceImage &img, const UniquePosition &swapTargetPos, const QSize &xyCount);
    ~GameDataSimpleSwap() = default;

    QString gameName() const override;
    PhasePointer createPhase(IPhase::PhaseType) override;
    PhasePointer createCurrentPhase() override;
    const SourceImage &sourceImage() const override;
    BoardInfoPointer boardInfo() const override;

    bool save(const QString &fileName) const override;
    bool load(const QString &fileName) override;

protected:
    void initPieces();
    void setAnimationToPieces();
    void setEffectToPieces();
    Fifteen::PuzzlePiecePointer &getPiece(const QPoint &pos);

    static const unsigned char slideFrameCount = 20;

    QList<Fifteen::PuzzlePiecePointer> pieces;
    SourceImage sourceImg;
    BoardPointer board;
    UniquePosition swapTargetPos;
    IPhase::PhaseType currentPhaseType;
};

#endif // GAMEDATASIMPLESLIDE_H
