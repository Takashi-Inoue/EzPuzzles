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

#include "SaveDataFifteen.h"

#include "BoardInformation.h"
#include "SourceImage.h"

#include <QFile>
#include <QSaveFile>

namespace Fifteen {

SaveDataFifteen::SaveDataFifteen(QStringView fileName, QObject *parent)
    : AbstractSaveData(fileName, parent)
{
    Q_ASSERT(!fileName.isEmpty());
}

SaveDataFifteen::SaveDataFifteen(QStringView fileName, const QSize &boardXYCount
                               , const UniquePosition &specifiedPosition
                               , const SourceImage &sourceImage
                               , AbstractPhase::PhaseType currentPhase
                               , const QList<QPoint> &defaultPositions, QObject *parent)
    : AbstractSaveData(fileName, sourceImage.fullPath(), sourceImage.pixmap()
                     , boardXYCount, currentPhase, parent)
    , m_specifiedPosition(specifiedPosition)
    , m_defaultPositions(defaultPositions)
{
    Q_ASSERT(!fileName.isEmpty());
    Q_ASSERT(!boardXYCount.isEmpty());
    Q_ASSERT(!sourceImage.isNull());
    Q_ASSERT(!defaultPositions.isEmpty());
}

QList<QPoint> SaveDataFifteen::defaultPositions() const
{
    return m_defaultPositions;
}

UniquePosition SaveDataFifteen::specifiedPosition() const
{
    return m_specifiedPosition;
}

void SaveDataFifteen::readInfo(QDataStream &stream)
{
    m_specifiedPosition.read(stream);
    m_isSavedataValid = (stream.status() == QDataStream::Ok);
}

void SaveDataFifteen::readOtherData(QDataStream &stream)
{
    stream >> m_defaultPositions;

    m_isSavedataValid = (stream.status() == QDataStream::Ok);
}

void SaveDataFifteen::writeInfo(QDataStream &stream) const
{
    m_specifiedPosition.write(stream);
}

void SaveDataFifteen::writeOtherData(QDataStream &stream) const
{
    stream << m_defaultPositions;
}

} // namespace Fifteen
