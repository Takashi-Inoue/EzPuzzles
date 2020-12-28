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

#include "BoardInformation.h"
#include "IPhase.h"
#include "SourceImage.h"

#include "EzPuzzles.h"

#include <QFile>
#include <QSaveFile>
#include <QStringList>
#include <QIcon>
#include <QSharedPointer>

class IGame;
class BoardInformation;

class AbstractSaveData : public QObject
{
    Q_OBJECT
public:
    AbstractSaveData(QStringView fileName, QObject *parent = nullptr)
        : QObject(parent)
        , m_fileName(fileName.toString())
        , m_currentPhase(IPhase::PhaseReady)
    {
        Q_ASSERT(!fileName.isEmpty());
    }

    AbstractSaveData(QStringView fileName, QString sourceImageFullPathName
                   , QPixmap sourceImagePixmap, QSize boardXYCount
                   , IPhase::PhaseType currentPhase, QObject *parent = nullptr)
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

    virtual QString gameTypeName() const
    {
        return EzPuzzles::gameName(gameType());
    }

    virtual EzPuzzles::GameType gameType() const = 0;
    virtual QIcon gameTypeIcon() const = 0;
    virtual QSharedPointer<IGame> loadGame() = 0;
    virtual QStringList informations() const = 0;

    QSharedPointer<BoardInformation> boardInformation() const
    {
        Q_ASSERT(!m_boardXYCount.isEmpty());
        Q_ASSERT(!m_sourceImagePixmap.isNull());

        if (m_boardXYCount.isEmpty() || m_sourceImagePixmap.isNull())
            return nullptr;

        return QSharedPointer<BoardInformation>::create(m_boardXYCount, m_sourceImagePixmap.size());
    }

    IPhase::PhaseType currentPhase() const {return m_currentPhase;}
    bool isValid() const {return m_isSavedataValid;}

    SourceImage sourceImage() const
    {
        if (m_sourceImageFullPathName.isEmpty() && m_sourceImagePixmap.isNull())
            return SourceImage();

        return SourceImage(m_sourceImageFullPathName, m_sourceImagePixmap);
    }

    virtual bool readInfo()
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

    virtual bool read()
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

    virtual bool write() const
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
