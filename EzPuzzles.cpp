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
#include "EzPuzzles.h"

#include <QCoreApplication>
#include <QDir>

QSize EzPuzzles::screenshotSize()
{
    return QSize(256, 256);
}

QString EzPuzzles::imageHistoryPath()
{
    return QCoreApplication::applicationDirPath() + "/" + "imageHistory.ini";
}

QString EzPuzzles::saveDirPath()
{
    return QCoreApplication::applicationDirPath() + "/" + "savedata";
}

void EzPuzzles::createSaveDirPath()
{
    QDir().mkpath(saveDirPath());
}
