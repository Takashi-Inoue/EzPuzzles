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
#ifndef ABSTRACTSAVEDATA_H
#define ABSTRACTSAVEDATA_H

#include "Application.h"
#include "IPhase.h"
#include "SourceImage.h"

#include <QStringList>
#include <QIcon>
#include <QSharedPointer>

class IGame;
class BoardInformation;

class AbstractSaveData : public QObject
{
    Q_OBJECT
public:
    AbstractSaveData(QStringView fileName, QObject *parent = nullptr);
    AbstractSaveData(QStringView fileName, QString sourceImageFullPathName
                   , QPixmap sourceImagePixmap, QSize boardXYCount
                   , IPhase::PhaseType currentPhase, QObject *parent = nullptr);

    virtual Application::GameType gameType() const = 0;
    virtual QIcon gameTypeIcon() const = 0;
    virtual QSharedPointer<IGame> loadGame() = 0;
    virtual QStringList informations() const = 0;

    virtual QString gameTypeName() const;

    QSharedPointer<BoardInformation> boardInformation() const;

    IPhase::PhaseType currentPhase() const;
    bool isValid() const;

    SourceImage sourceImage() const;

    virtual bool readInfo();
    virtual bool read();
    virtual bool write() const;

protected:
    virtual void readInfo(QDataStream &) {}
    virtual void readOtherData(QDataStream &) {}
    virtual void writeInfo(QDataStream &) const {}
    virtual void writeOtherData(QDataStream &) const {}

    const QString m_fileName;
    QString m_sourceImageFullPathName;
    QPixmap m_sourceImagePixmap;
    QSize m_boardXYCount;
    IPhase::PhaseType m_currentPhase;

    bool m_isSavedataValid = false;
};

#endif // ABSTRACTSAVEDATA_H
