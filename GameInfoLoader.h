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
#ifndef GAMEINFOLOADER_H
#define GAMEINFOLOADER_H

#include "ThreadOperation.h"

class ISaveData;

class GameInfoLoader : public ThreadOperation
{
    Q_OBJECT
public:
    GameInfoLoader(const QStringList &savedataNames);
    ~GameInfoLoader() = default;

signals:
    void loaded(QString savedataName, ISaveData *gameInfo);
    void failedToLoad(QString savedataName);

protected:
    QString className() const override;
    void execImpl() override;

private:
    ISaveData *createSaveData(const QString &gameName, const QString &savedataFilePath) const;

    QStringList savedataNames;
};

#endif // GAMEINFOLOADER_H
