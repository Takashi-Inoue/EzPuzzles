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

#include "ActionSelectImageFromHistory.h"

#include "Application.h"
#include "DialogImageHistory.h"

#include <QFileDialog>
#include <QSettings>

ActionSelectImageFromHistory::ActionSelectImageFromHistory(const QIcon &icon, const QString &text
                                                     , QObject *parent)
    : AbstractSelectImageAction(icon, text, parent)
{
    setToolTip(QStringLiteral("Select an image from history."));
}

ActionSelectImageFromHistory::ActionSelectImageFromHistory(const QString &text, QObject *parent)
    : ActionSelectImageFromHistory(QIcon(":/icons/history"), text, parent)
{
}

ActionSelectImageFromHistory::ActionSelectImageFromHistory(QObject *parent)
    : ActionSelectImageFromHistory(QString(), parent)
{
}

void ActionSelectImageFromHistory::onTriggered(bool /*checked*/)
{
    DialogImageHistory dialog(parentWidget());

    if (dialog.exec() != QDialog::Accepted)
        return;

    emit imageSelected(dialog.selectedImagePath());
}
