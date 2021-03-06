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
#ifndef PHASESIMPLESLIDEENDING_H
#define PHASESIMPLESLIDEENDING_H

#include "AbstractPhase.h"
#include "Fifteen/IBoard.h"
#include "Fifteen/PuzzlePiece.h"

namespace Slide {

class PhaseSimpleSlideEnding : public AbstractPhase
{
public:
    PhaseSimpleSlideEnding(QSharedPointer<Fifteen::IBoard> board, PhaseType nextPhase
                         , QObject *parent = nullptr);

    void click(const QPoint &) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    QString information() const override;

private:
    QSharedPointer<Fifteen::IBoard> m_board;
};

} // Slide

#endif // PHASESIMPLESLIDEENDING_H
