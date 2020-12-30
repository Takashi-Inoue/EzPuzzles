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
#include "Application.h"

#include <QDir>
#include <QFont>
#include <QSettings>

#include <QDebug>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    QApplication::setFont(QApplication::font("QMenu"));

    if (!createUserDataDir())
        qInfo() << QStringLiteral("Failed to create userdata directory. [%1]").arg(userDataDirPath());
}

QString Application::gameName(Application::GameType gameType)
{
    if (gameType == SimpleSlide)
        return QStringLiteral("Simple Slide");

    if (gameType == SimpleSwap)
        return QStringLiteral("Simple Swap");

    if (gameType == MineSweeper)
        return QStringLiteral("MineSweeper");

    return QString();
}

QSize Application::screenshotSize()
{
    return QSize(256, 256);
}

QString Application::iniFilePathName()
{
    return QCoreApplication::applicationDirPath() + QStringLiteral("/EzPuzzles.ini");
}

QString Application::userDataDirPath()
{
    return QCoreApplication::applicationDirPath() + QStringLiteral("/savedata");
}

bool Application::createUserDataDir()
{
    return QDir().mkpath(userDataDirPath());
}
