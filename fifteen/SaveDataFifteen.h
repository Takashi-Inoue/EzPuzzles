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

#ifndef FIFTEEN_SAVEDATAFIFTEEN_H
#define FIFTEEN_SAVEDATAFIFTEEN_H

#include "AbstractSaveData.h"

#include "AbstractPhase.h"
#include "UniquePosition.h"

namespace Fifteen {

class SaveDataFifteen : public AbstractSaveData
{
public:
    SaveDataFifteen(QStringView fileName, QObject *parent = nullptr);
    SaveDataFifteen(QStringView fileName, const QSize &boardXYCount
                  , const UniquePosition &specifiedPosition, const SourceImage &sourceImage
                  , AbstractPhase::PhaseType currentPhase, const QList<QPoint> &defaultPositions
                  , QObject *parent = nullptr);

    // Need to define in inherited class
    /*
    virtual EzPuzzles::GameType gameType() const = 0;
    virtual QIcon gameTypeIcon() const = 0;

    virtual IGame *loadGame() = 0;

    virtual QStringList informations() const = 0;
    */

    QList<QPoint> defaultPositions() const;
    UniquePosition specifiedPosition() const;

protected:
    void readInfo(QDataStream &stream) override;
    void readOtherData(QDataStream &) override;
    void writeInfo(QDataStream &) const override;
    void writeOtherData(QDataStream &) const override;

    UniquePosition m_specifiedPosition;
    QList<QPoint> m_defaultPositions;
};

} // namespace Fifteen

#endif // FIFTEEN_SAVEDATAFIFTEEN_H
