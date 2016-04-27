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

#include "MainWindow.h"
#include "EzPuzzles.h"

#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QTranslator>
#include <QLibraryInfo>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("tino");
    QCoreApplication::setApplicationName("EzPuzzles");

    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    QTranslator translator1;
    translator1.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QTranslator translator2;
    translator2.load("ezpz_" + QLocale::system().name(), QApplication::applicationDirPath());

    a.installTranslator(&translator1);
    a.installTranslator(&translator2);

    QApplication::setFont(QFont("Meiryo UI", QFontDatabase::systemFont(QFontDatabase::GeneralFont).pointSize()));

    QDir().mkpath(EzPuzzles::saveDirPath());

    MainWindow w;
    w.show();

    return a.exec();
}
