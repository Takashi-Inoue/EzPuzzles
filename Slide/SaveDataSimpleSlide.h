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
#ifndef SAVEDATASIMPLESLIDE_H
#define SAVEDATASIMPLESLIDE_H

#include "fifteen/SaveDataFifteen.h"
#include "SourceImage.h"
#include "UniquePosition.h"
#include "IPhase.h"

#include <QDataStream>
#include <QIcon>
#include <QList>
#include <QPoint>

namespace Slide {

class SaveDataSimpleSlide : public Fifteen::SaveDataFifteen
{
public:
    SaveDataSimpleSlide(QStringView fileName, QObject *parent = nullptr);
    SaveDataSimpleSlide(QStringView fileName, const QSize &boardXYCount
                      , const UniquePosition &specifiedPosition, const SourceImage &sourceImage
                      , IPhase::PhaseType currentPhase, const QList<QPoint> &defaultPositions
                      , const QPoint &currentBlankPos, QObject *parent = nullptr);

    EzPuzzles::GameType gameType() const override;
    QIcon gameTypeIcon() const override;

    QSharedPointer<IGame> loadGame() override;

    QStringList informations() const override;

    QPoint currentBlankPosition() const;

protected:
    void readInfo(QDataStream &stream) override;
    void writeInfo(QDataStream &) const override;

    QPoint m_currentBlankPos;
};

} // Slide

#endif // SAVEDATASIMPLESLIDE_H
