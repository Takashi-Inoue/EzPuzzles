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

void StringListHistory::load(QStringView iniFilePath, QString section, QString key)
{
    QSettings settings(iniFilePath.toString(), QSettings::IniFormat);

    if (!section.isEmpty())
        key.prepend(QString("%1/").arg(section));

    m_strings = settings.value(key, QStringList()).toStringList();
}

void StringListHistory::save(QStringView iniFilePath, QString section, QString key)
{
    QSettings settings(iniFilePath.toString(), QSettings::IniFormat);

    if (!section.isEmpty())
        key.prepend(QString("%1/").arg(section));

    settings.setValue(key, m_strings);
}

void StringListHistory::addString(QStringView string, bool isUnique)
{
    m_strings.push_front(string.toString());

    if (isUnique)
        m_strings.removeDuplicates();
}

void StringListHistory::addStrings(const QStringList &strings, bool isUnique)
{
    m_strings = strings + m_strings;

    if (isUnique)
        m_strings.removeDuplicates();
}

const QStringList &StringListHistory::stringList() const
{
    return m_strings;
}
