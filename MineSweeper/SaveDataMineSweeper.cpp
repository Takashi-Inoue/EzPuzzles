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
#include "SaveDataMineSweeper.h"
#include "GameDataMineSweeper.h"
#include "GameCoreMineSweeper.h"

#include "Application.h"

#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

namespace MineSweeper {

SaveDataMineSweeper::SaveDataMineSweeper(QStringView fileName, QObject *parent)
    : AbstractSaveData(fileName, parent)
{
}

SaveDataMineSweeper::SaveDataMineSweeper(QStringView fileName, const QSize &boardXYCount
                                       , int mineCount, int openedCount, int missedCount
                                       , bool isAutoLock, SourceImage sourceImage
                                       , IPhase::PhaseType currentPhaseType
                                       , const QList<int> &pieces, QObject *parent)
    : AbstractSaveData(fileName, sourceImage.fullPath(), sourceImage.pixmap()
                     , boardXYCount, currentPhaseType, parent)
    , m_mineCount(mineCount)
    , m_openedCount(openedCount)
    , m_missedCount(missedCount)
    , m_isAutoLock(isAutoLock)
    , m_pieces(pieces)
{
    Q_ASSERT(!boardXYCount.isEmpty());
}

Application::GameType SaveDataMineSweeper::gameType() const
{
    return Application::MineSweeper;
}

QIcon SaveDataMineSweeper::gameTypeIcon() const
{
    static QIcon icon(QStringLiteral(":/icons/mine"));
    return icon;
}

QSharedPointer<IGame> SaveDataMineSweeper::loadGame()
{
    if (!read())
        return nullptr;

    const GameID &gameID = GameID::fromString(QFileInfo(m_fileName).completeBaseName());

    return QSharedPointer<GameCoreMineSweeper>::create(
                QSharedPointer<GameDataMineSweeper>::create(*this), gameID);
}

QStringList SaveDataMineSweeper::informations() const
{
    int cellCount = m_boardXYCount.width() * m_boardXYCount.height();
    int safePieceCount = cellCount - m_mineCount;
    double openRate = (m_openedCount * 100.0) / safePieceCount;
    double mineRate = (m_mineCount * 100.0) / cellCount;
    QString autoLock = m_isAutoLock ? QStringLiteral("On") : QStringLiteral("Off");

    return {
        QStringLiteral("W%1 x H%2 : %3 cells")
                .arg(m_boardXYCount.width()).arg(m_boardXYCount.height()).arg(cellCount),
        QString(),
        QStringLiteral("%1 mines (%2%)").arg(m_mineCount).arg(mineRate, 0, 'f', 2),
        QStringLiteral("%1/%2 opened (%3%), %4 missed")
                .arg(m_openedCount).arg(safePieceCount).arg(openRate, 0, 'f', 2).arg(m_missedCount),
        QString(),
        QStringLiteral("Autolock naked mines : %1").arg(autoLock),
    };
}

int SaveDataMineSweeper::mineCount() const
{
    return m_mineCount;
}

void SaveDataMineSweeper::readInfo(QDataStream &stream)
{
    stream >> m_mineCount;
    stream >> m_openedCount;
    stream >> m_missedCount;
    stream >> m_isAutoLock;

    m_isSavedataValid = (stream.status() == QDataStream::Ok);
}

void SaveDataMineSweeper::readOtherData(QDataStream &stream)
{
    stream >> m_pieces;

    m_isSavedataValid = (stream.status() == QDataStream::Ok);
}

void SaveDataMineSweeper::writeInfo(QDataStream &stream) const
{
    stream << m_mineCount;
    stream << m_openedCount;
    stream << m_missedCount;
    stream << m_isAutoLock;
}

void SaveDataMineSweeper::writeOtherData(QDataStream &stream) const
{
    stream << m_pieces;
}

} // MineSweeper
