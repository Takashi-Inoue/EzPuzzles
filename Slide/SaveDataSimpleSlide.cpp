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
#include "SaveDataSimpleSlide.h"
#include "EzPuzzles.h"

#include <QSaveFile>

SaveDataSimpleSlide::SaveDataSimpleSlide(const QString &fileName) :
    fileName(fileName),
    isSavedataValid(true)
{
    Q_ASSERT(!fileName.isEmpty());
}

QIcon SaveDataSimpleSlide::gameTypeIcon() const
{
    return QIcon(":/ico/gameSimpleSlide");
}

bool SaveDataSimpleSlide::isValid() const
{
    return isSavedataValid;
}

bool SaveDataSimpleSlide::loadInfo()
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        isSavedataValid = false;
        return false;
    }

    return loadInfo(QDataStream(&file));
}

QString SaveDataSimpleSlide::gameName() const
{
    return gameTypeName;
}

QString SaveDataSimpleSlide::imageFilePath() const
{
    return sourceImg.fullPath;
}

QStringList SaveDataSimpleSlide::informations() const
{
    return {
        QString("W%1 x H%2 : %3 pieces").arg(boardSize.width()).arg(boardSize.height()).arg(boardSize.width() * boardSize.height()),
        "",
        QString("Default blank ") + defaultBlankPos.toString(),
    };
}

bool SaveDataSimpleSlide::save() const
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream stream(&file);

    stream << gameTypeName;
    stream << boardSize;
    defaultBlankPos.write(stream);
    stream << currentBlankPos;
    stream << sourceImg.fullPath;
    stream << sourceImg.pixmap;
    stream << static_cast<qint8>(currentPhaseType);
    stream << defaultPositions;

    return file.commit();
}

bool SaveDataSimpleSlide::loadInfo(QDataStream &stream)
{
    stream >> gameTypeName;

    if (gameTypeName != EzPuzzles::gameName(EzPuzzles::SimpleSlide)) {
        isSavedataValid = false;
        return false;
    }

    stream >> boardSize;
    defaultBlankPos.read(stream);
    stream >> currentBlankPos;
    stream >> sourceImg.fullPath;

    isSavedataValid = (stream.status() == QDataStream::Ok);

    return isSavedataValid;
}

bool SaveDataSimpleSlide::load()
{
    QSaveFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        isSavedataValid = false;
        return false;
    }

    QDataStream stream(&file);

    if (!loadInfo(stream))
        return false;

    stream >> sourceImg.pixmap;

    qint8 phaseType;

    stream >> phaseType;

    currentPhaseType = static_cast<IPhase::PhaseType>(phaseType);

    stream >> defaultPositions;

    isSavedataValid = (stream.status() == QDataStream::Ok);

    return isSavedataValid;
}
