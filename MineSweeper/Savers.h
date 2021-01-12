/*
 * Copyright 2021 Takashi Inoue
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

#ifndef MINESWEEPER_SAVERS_H
#define MINESWEEPER_SAVERS_H

#include <QString>

namespace MineSweeper {

class Savers
{
public:
    Savers(uchar maximum, ushort pointsRequiredToObtain);
    ~Savers() = default;

    void clear();
    bool consume();
    void incrementPoint();

    bool hasSaver() const;
    QString information() const;
    uchar maximum() const;

private:
    uchar m_maximum;
    ushort m_pointsRequiredToObtain;

    uchar m_saversCount = 0;
    ushort m_currentPoints = 0;
};

} // namespace MineSweeper

#endif // MINESWEEPER_SAVERS_H
