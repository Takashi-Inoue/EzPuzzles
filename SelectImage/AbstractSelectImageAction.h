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

#ifndef ABSTRACTSELECTIMAGEACTION_H
#define ABSTRACTSELECTIMAGEACTION_H

#include <QAction>

class AbstractSelectImageAction : public QAction
{
    Q_OBJECT
public:
    AbstractSelectImageAction(const QIcon &icon, const QString &text, QObject *parent = nullptr)
        : QAction(icon, text, parent)
    {
        connect(this, &QAction::triggered, this, &AbstractSelectImageAction::onTriggered);
    }

    AbstractSelectImageAction(const QString &text, QObject *parent = nullptr)
        : AbstractSelectImageAction(QIcon(), text, parent) {}
    AbstractSelectImageAction(QObject *parent = nullptr)
        : AbstractSelectImageAction(QIcon(), QString(), parent) {}

    virtual void save() {}

signals:
    void imageSelected(QString imageFullPathName);

protected slots:
    virtual void onTriggered(bool checked = false) = 0;
};

#endif // ABSTRACTSELECTIMAGEACTION_H
