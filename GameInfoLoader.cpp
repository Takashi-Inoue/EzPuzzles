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
#include "GameInfoLoader.h"
#include "EzPuzzles.h"
#include "MineSweeper/SaveDataMineSweeper.h"
#include "Slide/SaveDataSimpleSlide.h"
#include "Swap/SaveDataSimpleSwap.h"
#include "BrokenSaveData.h"
#include "UnknownSaveData.h"

#include <QDir>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>

GameInfoLoader::GameInfoLoader(const QStringList &savedataNames) :
    ThreadOperation(nullptr),
    savedataNames(savedataNames)
{
}

QString GameInfoLoader::className() const
{
    return "GameInfoLoader";
}

void GameInfoLoader::execImpl()
{
    QString savedataDir = EzPuzzles::saveDirPath() + "/";

    for (auto &name : savedataNames) {
        if (isStopped()) {
            qInfo() << "--ABORTED loading savedata info : class" << className();
            return;
        }

        QFile file(savedataDir + name);

        if (!file.open(QIODevice::ReadOnly)) {
            emit failedToLoad(name);
            continue;
        }

        QDataStream stream(&file);

        QString gameName;

        stream >> gameName;

        auto gameInfo = createSaveData(gameName, savedataDir + name);

        if (!gameInfo->loadInfo()) {
            delete gameInfo;

            gameInfo = new BrokenSaveData();
        }

        emit loaded(name, gameInfo);
    }
}

ISaveData *GameInfoLoader::createSaveData(const QString &gameName, const QString &savedataFilePath) const
{
    if (gameName == EzPuzzles::gameName(EzPuzzles::SimpleSlide))
        return new SaveDataSimpleSlide(savedataFilePath);

    if (gameName == EzPuzzles::gameName(EzPuzzles::SimpleSwap))
        return new SaveDataSimpleSwap(savedataFilePath);

    if (gameName == EzPuzzles::gameName(EzPuzzles::MineSweeper))
        return new MineSweeper::SaveDataMineSweeper(savedataFilePath);

    return new UnknownSaveData();
}
