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
#include "PhaseSimpleSwapEnding.h"

namespace Swap {

PhaseSimpleSwapEnding::PhaseSimpleSwapEnding(QSharedPointer<Fifteen::IBoard> board
                                           , PhaseType nextPhase, QObject *parent)
    : AbstractPhase(nextPhase, parent)
    , m_board(board)
{
    Q_CHECK_PTR(board);
}

void PhaseSimpleSwapEnding::click(const QPoint &)
{
    emit toNextPhase(m_nextPhaseType);
}

void PhaseSimpleSwapEnding::onTickFrame()
{
    if (!m_board->onTickFrame())
        emit toNextPhase(m_nextPhaseType);
}

void PhaseSimpleSwapEnding::draw(QPainter &painter)
{
    m_board->draw(painter);
}

bool PhaseSimpleSwapEnding::canSave() const
{
    return false;
}

bool PhaseSimpleSwapEnding::canLoad() const
{
    return false;
}

QString PhaseSimpleSwapEnding::information() const
{
    return QStringLiteral("Clear!");
}

} // Swap
