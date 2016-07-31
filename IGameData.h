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
#ifndef IGAMEDATA_H
#define IGAMEDATA_H

#include "EzPuzzles.h"
#include "IPhase.h"
#include "BoardInformation.h"

#include <memory>

class IGameData
{
public:
    virtual QString gameName() const = 0;
    virtual PhasePointer createPhase(IPhase::PhaseType) = 0;
    virtual const SourceImage &sourceImage() const = 0;
    virtual BoardInfoPointer boardInfo() const = 0;

    virtual void save(QDataStream &) const = 0;
    virtual void load(QDataStream &) = 0;
};

typedef std::shared_ptr<IGameData> GameDataPointer;

#endif // IPHASEFACTORY_H
