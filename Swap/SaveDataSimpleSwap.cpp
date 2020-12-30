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
#include "SaveDataSimpleSwap.h"
#include "GameDataSimpleSwap.h"
#include "GameCore.h"
#include "BoardInformation.h"
#include "Application.h"

#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

namespace Swap {

Application::GameType SaveDataSimpleSwap::gameType() const
{
    return Application::SimpleSwap;
}

QIcon SaveDataSimpleSwap::gameTypeIcon() const
{
    static QIcon icon(QStringLiteral(":/icons/swap"));
    return icon;
}

QSharedPointer<IGame> SaveDataSimpleSwap::loadGame()
{
    if (!read())
        return nullptr;

    const GameID &gameID = GameID::fromString(QFileInfo(m_fileName).completeBaseName());

    return QSharedPointer<GameCore>::create(QSharedPointer<GameDataSimpleSwap>::create(*this), gameID);
}

QStringList SaveDataSimpleSwap::informations() const
{
    return {
        QStringLiteral("W%1 x H%2 : %3 pieces")
                .arg(m_boardXYCount.width()).arg(m_boardXYCount.height())
                .arg(m_boardXYCount.width() * m_boardXYCount.height()),
        QString(),
        QStringLiteral("Swap target %1").arg(m_specifiedPosition.toString()),
    };
}

} // Swap
