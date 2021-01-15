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
#include "PhaseMineSweeperGaming.h"

#include <QDebug>

namespace MineSweeper {

PhaseMineSweeperGaming::PhaseMineSweeperGaming(MineFieldPointer mineField, PhaseType nextPhase, QObject *parent)
    : AbstractPhase(nextPhase, parent)
    , m_mineField(mineField)
{
    Q_CHECK_PTR(mineField);
}

void PhaseMineSweeperGaming::click(const QPoint &clickedPiecePos)
{
    if (m_mineField->isAllOpened() & !m_mineField->isNoMissed()) {
        emit toNextPhase(AbstractPhase::PhaseReady);
        return;
    }

    m_mineField->open(clickedPiecePos);

    if (m_mineField->isAllOpened() & m_mineField->isNoMissed())
        emit toNextPhase(m_nextPhaseType);
}

void PhaseMineSweeperGaming::press(const QPoint &clickedPiecePos)
{
    m_mineField->press(clickedPiecePos);
}

void PhaseMineSweeperGaming::release(const QPoint &clickedPiecePos)
{
    m_mineField->release(clickedPiecePos);
}

void PhaseMineSweeperGaming::draw(QPainter &painter)
{
    m_mineField->draw(painter);
}

bool PhaseMineSweeperGaming::canSave() const
{
    return true;
}

bool PhaseMineSweeperGaming::canLoad() const
{
    return true;
}

QString PhaseMineSweeperGaming::information() const
{
    return m_mineField->information();
}

} // MineSweeper
