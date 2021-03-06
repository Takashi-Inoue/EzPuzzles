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

#include "Savers.h"

namespace MineSweeper {

Savers::Savers(uchar maximum, ushort pointsRequiredToObtain)
    : m_maximum(maximum)
    , m_pointsRequiredToObtain(pointsRequiredToObtain)
{
}

void Savers::clear()
{
    m_currentPoints = 0;
}

bool Savers::consume()
{
    if (m_saversCount == 0)
        return false;

    --m_saversCount;
    m_maximum = qMax<uchar>(0, m_maximum - 1);

    return true;
}

void Savers::incrementPoint()
{
    if (m_pointsRequiredToObtain == 0)
        return;

    if (++m_currentPoints < m_pointsRequiredToObtain)
        return;

    m_currentPoints = 0;

    if (m_saversCount < m_maximum && ++m_saversCount == m_maximum)
        m_pointsRequiredToObtain = 0;
}

bool Savers::hasSaver() const
{
    return m_saversCount > 0;
}

QString Savers::information() const
{
    return QStringLiteral("Savers (%1 / %2) [%3 / %4]")
            .arg(m_saversCount).arg(m_maximum).arg(m_currentPoints).arg(m_pointsRequiredToObtain);
}

uchar Savers::maximum() const
{
    return m_maximum;
}

QString Savers::savedataInfo() const
{
    return QStringLiteral("Savers: obtained (%1 / %2)").arg(m_saversCount).arg(m_maximum);
}

QDataStream &Savers::read(QDataStream &in)
{
    in >> m_maximum
       >> m_pointsRequiredToObtain
       >> m_saversCount
       >> m_currentPoints;

    return in;
}

QDataStream &Savers::write(QDataStream &out) const
{
    out << m_maximum
        << m_pointsRequiredToObtain
        << m_saversCount
        << m_currentPoints;

    return out;
}

} // namespace MineSweeper

QDataStream &operator<<(QDataStream &out, const MineSweeper::Savers &savers)
{
    return savers.write(out);
}

QDataStream &operator>>(QDataStream &in, MineSweeper::Savers &savers)
{
    return savers.read(in);
}
