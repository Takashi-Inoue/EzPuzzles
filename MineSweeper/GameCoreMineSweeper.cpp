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
#include "GameCoreMineSweeper.h"

#include <QDebug>

namespace MineSweeper {

QSharedPointer<IGame> GameCoreMineSweeper::cloneAsNewGame() const
{
    auto game = new GameCoreMineSweeper(m_gameData->cloneAsNewGame());

    const_cast<GameID *>(&m_gameId)->swap(*const_cast<GameID *>(&game->m_gameId));

    return QSharedPointer<IGame>(game);
}

void GameCoreMineSweeper::mouseMove(const QSize &fieldSize, const QPoint &cursorPos)
{
    QPoint pressedPiecePos = piecePosFromCursorPos(fieldSize, cursorPos);

    if (pressedPiecePos == m_pressedPiecePos)
        return;

    m_phase->release(m_pressedPiecePos);
    m_phase->press(pressedPiecePos);

    m_pressedPiecePos = pressedPiecePos;
}

void GameCoreMineSweeper::mousePress(const QSize &fieldSize, const QPoint &cursorPos)
{
    mouseMove(fieldSize, cursorPos);
}

void GameCoreMineSweeper::mouseRelease(const QSize &fieldSize, const QPoint &cursorPos)
{
    m_phase->click(piecePosFromCursorPos(fieldSize, cursorPos));

    emit informationUpdated(shortInformation());
}

QPoint GameCoreMineSweeper::piecePosFromCursorPos(const QSize &fieldSize, const QPoint &cursorPos) const
{
    return GameCore::piecePosFromCursorPos(fieldSize, cursorPos) + QPoint(1, 1);
}

} // MineSweeper
