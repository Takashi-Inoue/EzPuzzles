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
#ifndef SAVEINFOLOADER_H
#define SAVEINFOLOADER_H

#include "ThreadOperation.h"

class SaveInfoLoader : public ThreadOperation
{
    Q_OBJECT
public:
    SaveInfoLoader(const QStringList &savedataNames);
    ~SaveInfoLoader() = default;

signals:
    void loaded(QString savedataName, QString gameName, QString imageBaseName);
    void failedToLoad(QString savedataName);

protected:
    QString className() const override;
    void execImpl() override;

private:
    QStringList savedataNames;
};

#endif // SAVEINFOLOADER_H
