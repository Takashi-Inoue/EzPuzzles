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

namespace MineSweeper {

QSharedPointer<IGame> GameCoreMineSweeper::cloneAsNewGame() const
{
    auto game = new GameCoreMineSweeper(m_gameData->cloneAsNewGame());

    const_cast<GameID *>(&m_gameId)->swap(*const_cast<GameID *>(&game->m_gameId));

    return QSharedPointer<IGame>(game);
}

void GameCoreMineSweeper::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    const auto &piecePos = piecePosFromCursorPos(fieldSize, cursorPos);

    m_phase->click(piecePos + QPoint(1, 1));

    emit informationUpdated(shortInformation());
}

bool GameCoreMineSweeper::hasFinalImage() const
{
    return false;
}

} // MineSweeper
