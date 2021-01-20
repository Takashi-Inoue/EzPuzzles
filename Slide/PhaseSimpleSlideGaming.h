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
#ifndef PHASESIMPLESLIDEGAMING_H
#define PHASESIMPLESLIDEGAMING_H

#include "AbstractPhase.h"
#include "Fifteen/AbstractGameBoard.h"
#include "Fifteen/PuzzlePiece.h"

namespace Slide {

class PhaseSimpleSlideGaming : public AbstractPhase
{
    Q_OBJECT
public:
    PhaseSimpleSlideGaming(GameBoardPtr board, QPoint &currentBlankPos
                         , const QPoint &defaultBlankPos, PhaseType nextPhase, QObject *parent = nullptr);

    void click(const QPoint &clickedPiecePos) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

protected:
    GameBoardPtr m_board;
    QPoint &m_currentBlankPos;
    const QPoint m_defaultBlankPos;
    bool m_isGameCleared;
};

} // Slide

#endif // PHASESIMPLESLIDEGAMING_H
