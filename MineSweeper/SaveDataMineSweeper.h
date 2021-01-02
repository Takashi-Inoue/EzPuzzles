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
#ifndef SAVEDATAMINESWEEPER_H
#define SAVEDATAMINESWEEPER_H

#include "AbstractSaveData.h"
#include "SourceImage.h"
#include "IPhase.h"

#include <QDataStream>
#include <QIcon>
#include <QList>

namespace MineSweeper {

class SaveDataMineSweeper : public AbstractSaveData
{
public:
    SaveDataMineSweeper(QStringView fileName, QObject *parent = nullptr);
    SaveDataMineSweeper(QStringView fileName, const QSize &boardXYCount, int mineCount
                      , int openedCount, int missedCount, bool isAutoLock
                      , SourceImage sourceImage, IPhase::PhaseType currentPhaseType
                      , const QList<int> &pieces, QObject *parent = nullptr);

    Application::GameType gameType() const override;
    QIcon gameTypeIcon() const override;

    QSharedPointer<IGame> loadGame() override;

    QStringList informations() const override;

    //--
    bool isAutoLock() const;
    int mineCount() const;
    int missedCount() const;
    int openedCount() const;
    QList<int> piecesAsIntList() const;

private:
    void readInfo(QDataStream &stream) override;
    void readOtherData(QDataStream &) override;
    void writeInfo(QDataStream &) const override;
    void writeOtherData(QDataStream &) const override;

    int m_mineCount;
    int m_openedCount;
    int m_missedCount;
    bool m_isAutoLock;
    QList<int> m_pieces;
};

} // MineSweeper

#endif // SAVEDATAMINESWEEPER_H
