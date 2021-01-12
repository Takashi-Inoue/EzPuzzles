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

#include "ImageHistory.h"
#include "Application.h"

#include <QSettings>

QList<int> ImageHistory::thumbnailSizeList()
{
    return {128, 192, 256, 320, 384, 448, 512};
}

int ImageHistory::thumbnailSize() const
{
    return m_thumbnailSize;
}

void ImageHistory::setThumbnailSize(int size)
{
    m_thumbnailSize = size;

    QList<int> sizeList = thumbnailSizeList();

    if (!sizeList.contains(m_thumbnailSize))
        m_thumbnailSize = sizeList.first();
}

void ImageHistory::load()
{
    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    settings.beginGroup(m_groupName);
    m_strings       = settings.value("Paths", QStringList()).toStringList();
    m_thumbnailSize = settings.value("ThumbnailSize", 128).toInt();
    settings.endGroup();

    QList<int> sizeList = thumbnailSizeList();

    if (!sizeList.contains(m_thumbnailSize))
        m_thumbnailSize = sizeList.first();
}

void ImageHistory::save()
{
    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    settings.beginGroup(m_groupName);
    settings.setValue("Paths",         m_strings);
    settings.setValue("ThumbnailSize", m_thumbnailSize);
    settings.endGroup();
}
