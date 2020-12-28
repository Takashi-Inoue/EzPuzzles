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
#include "SaveDataSimpleSlide.h"
#include "GameDataSimpleSlide.h"
#include "GameCore.h"
#include "EzPuzzles.h"

#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

namespace Slide {

SaveDataSimpleSlide::SaveDataSimpleSlide(QStringView fileName, QObject *parent)
    : SaveDataFifteen(fileName, parent)
{
}

SaveDataSimpleSlide::SaveDataSimpleSlide(QStringView fileName, const QSize &boardXYCount
                                       , const UniquePosition &specifiedPosition
                                       , const SourceImage &sourceImage
                                       , IPhase::PhaseType currentPhase
                                       , const QList<QPoint> &defaultPositions
                                       , const QPoint &currentBlankPos, QObject *parent)
    : SaveDataFifteen(fileName, boardXYCount, specifiedPosition, sourceImage
                    , currentPhase, defaultPositions, parent)
    , m_currentBlankPos(currentBlankPos)
{
}

EzPuzzles::GameType SaveDataSimpleSlide::gameType() const
{
    return EzPuzzles::SimpleSlide;
}

QIcon SaveDataSimpleSlide::gameTypeIcon() const
{
    static QIcon icon(QStringLiteral(":/icons/slide"));
    return icon;
}

QSharedPointer<IGame> SaveDataSimpleSlide::loadGame()
{
    if (!read())
        return nullptr;

    const GameID &gameID = GameID::fromString(QFileInfo(m_fileName).completeBaseName());

    return QSharedPointer<GameCore>::create(QSharedPointer<GameDataSimpleSlide>::create(*this), gameID);
}

QStringList SaveDataSimpleSlide::informations() const
{
    return {
        QStringLiteral("W%1 x H%2 : %3 pieces")
                      .arg(m_boardXYCount.width()).arg(m_boardXYCount.height())
                      .arg(m_boardXYCount.width() * m_boardXYCount.height()),
        QString(),
        QStringLiteral("Default blank %1").arg(m_specifiedPosition.toString()),
    };
}

QPoint SaveDataSimpleSlide::currentBlankPosition() const
{
    return m_currentBlankPos;
}

void SaveDataSimpleSlide::readInfo(QDataStream &stream)
{
    SaveDataFifteen::readInfo(stream);

    if (!m_isSavedataValid)
        return;

    stream >> m_currentBlankPos;

    m_isSavedataValid = (stream.status() == QDataStream::Ok);
}

void SaveDataSimpleSlide::writeInfo(QDataStream &stream) const
{
    SaveDataFifteen::writeInfo(stream);

    stream << m_currentBlankPos;
}

} // Slide
