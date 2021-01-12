/*
 * Copyright 2021 Takashi Inoue
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

#ifndef ACTIONSELECTIMAGEFROMHISTORY_H
#define ACTIONSELECTIMAGEFROMHISTORY_H

#include "AbstractSelectImageAction.h"

class ActionSelectImageFromHistory : public AbstractSelectImageAction
{
    Q_OBJECT
public:
    ActionSelectImageFromHistory(const QIcon &icon, const QString &text, QObject *parent = nullptr);
    ActionSelectImageFromHistory(const QString &text, QObject *parent = nullptr);
    ActionSelectImageFromHistory(QObject *parent = nullptr);

protected slots:
    void onTriggered(bool checked = false) override;
};

#endif // ACTIONSELECTIMAGEFROMHISTORY_H
