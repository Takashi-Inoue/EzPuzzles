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

#include "ActionSelectImageFromLocal.h"

#include "Application.h"

#include <QFileDialog>
#include <QSettings>

ActionSelectImageFromLocal::ActionSelectImageFromLocal(const QIcon &icon, const QString &text
                                                     , QObject *parent)
    : AbstractSelectImageAction(icon, text, parent)
{
    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    m_imageDir = settings.value(m_settingsKey, QString()).toString();

    setToolTip(QStringLiteral("Select an image from local."));
}

ActionSelectImageFromLocal::ActionSelectImageFromLocal(const QString &text, QObject *parent)
    : ActionSelectImageFromLocal(QIcon(":/icons/plus"), text, parent)
{
}

ActionSelectImageFromLocal::ActionSelectImageFromLocal(QObject *parent)
    : ActionSelectImageFromLocal(QString(), parent)
{
}

void ActionSelectImageFromLocal::save()
{
    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    settings.setValue(m_settingsKey, m_imageDir);
}

void ActionSelectImageFromLocal::onTriggered(bool /*checked*/)
{
    QString imagePath = QFileDialog::getOpenFileName(
                            parentWidget()
                          , QStringLiteral("Open image..."), m_imageDir
                          , QStringLiteral("Images (*.png *.bmp *.jpg *.jpeg)"));

    if (imagePath.isEmpty())
        return;

    m_imageDir = QFileInfo(imagePath).absolutePath();

    save();

    emit imageSelected(imagePath);
}
