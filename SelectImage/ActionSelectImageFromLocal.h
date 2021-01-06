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

#ifndef ACTIONSELECTIMAGEFROMLOCAL_H
#define ACTIONSELECTIMAGEFROMLOCAL_H

#include "AbstractSelectImageAction.h"

class ActionSelectImageFromLocal : public AbstractSelectImageAction
{
    Q_OBJECT
public:
    ActionSelectImageFromLocal(const QIcon &icon, const QString &text, QObject *parent = nullptr);
    ActionSelectImageFromLocal(const QString &text, QObject *parent = nullptr);
    ActionSelectImageFromLocal(QObject *parent = nullptr);

    void save() override;

protected slots:
    void onTriggered(bool checked = false) override;

protected:
    static constexpr char m_settingsKey[] = "imageDir";
    static inline QString m_imageDir;
};

#endif // ACTIONSELECTIMAGEFROMLOCAL_H
