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
#ifndef SAVEDATASIMPLESWAP_H
#define SAVEDATASIMPLESWAP_H

#include "fifteen/SaveDataFifteen.h"

#include <QIcon>
#include <QList>
#include <QPoint>

namespace Swap {

class SaveDataSimpleSwap : public Fifteen::SaveDataFifteen
{
public:
    using Fifteen::SaveDataFifteen::SaveDataFifteen;

    // AbstractSaveData
    EzPuzzles::GameType gameType() const override;
    QIcon gameTypeIcon() const override;

    QSharedPointer<IGame> loadGame() override;

    QStringList informations() const override;
};

} // Swap

#endif // SAVEDATASIMPLESWAP_H
