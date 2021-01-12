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

#include "AbstractSaveData.h"
#include "BoardInformation.h"

#include <QFile>
#include <QSaveFile>

AbstractSaveData::AbstractSaveData(QStringView fileName, QObject *parent)
    : QObject(parent)
    , m_fileName(fileName.toString())
    , m_currentPhase(AbstractPhase::PhaseReady)
{
    Q_ASSERT(!fileName.isEmpty());
}

AbstractSaveData::AbstractSaveData(QStringView fileName, QString sourceImageFullPathName
                                 , QPixmap sourceImagePixmap, QSize boardXYCount
                                 , AbstractPhase::PhaseType currentPhase, QObject *parent)
    : QObject(parent)
    , m_fileName(fileName.toString())
    , m_sourceImageFullPathName(sourceImageFullPathName)
    , m_sourceImagePixmap(sourceImagePixmap)
    , m_boardXYCount(boardXYCount)
    , m_currentPhase(currentPhase)
{
    Q_ASSERT(!fileName.isEmpty());
    Q_ASSERT(!sourceImageFullPathName.isEmpty());
    Q_ASSERT(!sourceImagePixmap.isNull());
    Q_ASSERT(!boardXYCount.isEmpty());
}

QString AbstractSaveData::gameTypeName() const
{
    return Application::gameName(gameType());
}

QSharedPointer<BoardInformation> AbstractSaveData::boardInformation() const
{
    Q_ASSERT(!m_boardXYCount.isEmpty());
    Q_ASSERT(!m_sourceImagePixmap.isNull());

    if (m_boardXYCount.isEmpty() || m_sourceImagePixmap.isNull())
        return nullptr;

    return QSharedPointer<BoardInformation>::create(m_boardXYCount, m_sourceImagePixmap.size());
}

AbstractPhase::PhaseType AbstractSaveData::currentPhase() const
{
    return m_currentPhase;
}

bool AbstractSaveData::isValid() const
{
    return m_isSavedataValid;
}

SourceImage AbstractSaveData::sourceImage() const
{
    if (m_sourceImageFullPathName.isEmpty() && m_sourceImagePixmap.isNull())
        return SourceImage();

    return SourceImage(m_sourceImageFullPathName, m_sourceImagePixmap);
}

bool AbstractSaveData::readInfo()
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::ReadOnly))
        return (m_isSavedataValid = false);

    QDataStream stream(&file);
    QString gameName;

    stream >> gameName >> m_boardXYCount >> m_sourceImageFullPathName;

    if (gameName != gameTypeName())
        return (m_isSavedataValid = false);

    readInfo(stream);

    return m_isSavedataValid;
}

bool AbstractSaveData::read()
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::ReadOnly))
        return (m_isSavedataValid = false);

    QDataStream stream(&file);
    QString gameName;

    stream >> gameName >> m_boardXYCount >> m_sourceImageFullPathName;

    if (gameName != gameTypeName())
        return (m_isSavedataValid = false);

    readInfo(stream);

    stream >> m_sourceImagePixmap >> m_currentPhase;

    readOtherData(stream);

    return m_isSavedataValid;
}

bool AbstractSaveData::write() const
{
    QSaveFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream stream(&file);

    stream << gameTypeName() << m_boardXYCount << m_sourceImageFullPathName;

    writeInfo(stream);

    stream << m_sourceImagePixmap << m_currentPhase;

    writeOtherData(stream);

    return file.commit();
}
