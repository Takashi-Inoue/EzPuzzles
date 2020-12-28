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
#include "UniquePosition.h"

#include <QRandomGenerator>

void UniquePosition::randomSelect(const QSize &xyCount)
{
    Q_ASSERT(!xyCount.isEmpty());

    m_isRandomSelect = true;

    QRandomGenerator *randomGenerator = QRandomGenerator::global();

    m_selectedPos.rx() = randomGenerator->bounded(xyCount.width());
    m_selectedPos.ry() = randomGenerator->bounded(xyCount.height());
}

void UniquePosition::select(const QPoint &pos)
{
    m_isRandomSelect = false;
    m_selectedPos = pos;
}

const QPoint &UniquePosition::selectedPosition() const
{
    return m_selectedPos;
}

bool UniquePosition::isRandom() const
{
    return m_isRandomSelect;
}

void UniquePosition::read(QDataStream &stream)
{
    stream >> m_isRandomSelect >> m_selectedPos;
}

void UniquePosition::write(QDataStream &stream) const
{
    stream << m_isRandomSelect << m_selectedPos;
}

QString UniquePosition::toString() const
{
    return m_isRandomSelect ? QStringLiteral("[???]<Random>")
                            : QStringLiteral("[%1, %2]<Specified>")
                             .arg(m_selectedPos.x() + 1).arg(m_selectedPos.y() + 1);
}
