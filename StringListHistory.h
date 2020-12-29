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
#ifndef STRINGLISTHISTORY_H
#define STRINGLISTHISTORY_H

#include <QStringList>

class StringListHistory
{
public:
    StringListHistory() = default;
    ~StringListHistory() = default;

    void load(QStringView iniFilePath, QString section = QString(), QString key = QStringLiteral("history"));
    void save(QStringView iniFilePath, QString section = QString(), QString key = QStringLiteral("history"));

    void addString(QStringView string, bool isUnique = true);
    void addStrings(const QStringList &strings, bool isUnique = true);

    const QStringList &stringList() const;

private:
    QStringList m_strings;
};

#endif // STRINGLISTHISTORY_H
