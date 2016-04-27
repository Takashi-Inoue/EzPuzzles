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
#include "SaveManager.h"

#include "EzPuzzles.h"
#include "GameFifteen.h"
#include "GameSwap.h"
#include "mine/GameMineSweeper.h"

#include <QFileInfo>

QStringList SaveManager::filterExtensions()
{
    QStringList result = {
        "*." + GameFifteen::savedataExtension(),
        "*." + GameSwap::savedataExtension(),
        "*." + MineSweeper::GameMineSweeper::savedataExtension(),
    };

    return result;
}

void SaveManager::save(IGame *game)
{
    if (game == nullptr)
        return;

    EzPuzzles::createSaveDirPath();
    game->save(EzPuzzles::saveDirPath());
}

IGame *SaveManager::load(const QString &savedataPath)
{
    IGame *game = nullptr;

    QString ext = QFileInfo(savedataPath).completeSuffix();

    if (ext == MineSweeper::GameMineSweeper::savedataExtension())
        game = new MineSweeper::GameMineSweeper();

    if (game != nullptr && game->load(savedataPath))
        return game;

    delete game;

    return nullptr;
}
