﻿/*
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
#ifndef SAVEDATASIMPLESLIDE_H
#define SAVEDATASIMPLESLIDE_H

#include "SourceImage.h"
#include "UniquePosition.h"
#include "IPhase.h"

#include <QDataStream>
#include <QIcon>
#include <QList>
#include <QPoint>

class SaveDataSimpleSlide
{
public:
    SaveDataSimpleSlide(const QString &fileName);
    ~SaveDataSimpleSlide() = default;

    QIcon gameTypeIcon() const;
    bool isValid() const;

    bool loadInfo();

    QString gameName() const;
    QString imageFilePath() const;
    QStringList informations() const;

private:
    friend class GameDataSimpleSlide;

    bool save() const;
    bool loadInfo(QDataStream &stream);
    bool load();

    QString fileName;
    bool isSavedataValid;

    QString gameTypeName;
    SourceImage sourceImg;
    QSize boardSize;
    UniquePosition defaultBlankPos;
    QPoint currentBlankPos;
    IPhase::PhaseType currentPhaseType;
    QList<QPoint> defaultPositions;
};

#endif // SAVEDATASIMPLESLIDE_H
