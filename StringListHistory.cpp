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
#include "StringListHistory.h"

#include <QSettings>

void StringListHistory::load(const QString &iniFilePath)
{
    QSettings settings(iniFilePath, QSettings::IniFormat);

    strings = settings.value("paths", QStringList()).toStringList();
}

void StringListHistory::save(const QString &iniFilePath)
{
    QSettings settings(iniFilePath, QSettings::IniFormat);

    settings.setValue("paths", strings);
}

void StringListHistory::addString(const QString &string, bool isUnique)
{
    if (isUnique)
        strings.removeOne(string);

    strings.push_front(string);
}

void StringListHistory::addStrings(const QStringList &strings, bool isUnique)
{
    if (!isUnique) {
        for (auto &string : strings)
            this->strings.push_front(string);

        return;
    }

    // ソート不可なので一つ一つ調べる
    for (auto &string : strings) {
        if (!this->strings.contains(string))
            this->strings.push_front(string);
    }
}

const QStringList &StringListHistory::stringList() const
{
    return strings;
}
