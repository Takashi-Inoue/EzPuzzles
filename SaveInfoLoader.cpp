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
#include "SaveInfoLoader.h"
#include "EzPuzzles.h"

#include <QDir>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>

SaveInfoLoader::SaveInfoLoader(const QStringList &savedataNames) :
    ThreadOperation(nullptr),
    savedataNames(savedataNames)
{
}

QString SaveInfoLoader::className() const
{
    return "SaveInfoLoader";
}

void SaveInfoLoader::execImpl()
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
        QString imageFullPath;

        stream >> gameName >> imageFullPath;

        emit loaded(name, gameName, QFileInfo(imageFullPath).baseName());
    }
}
