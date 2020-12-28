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
#include "SourceImage.h"

#include <QFileDialog>
#include <QFileInfo>

SourceImage::SourceImage(QStringView imagePath)
    : SourceImage(imagePath, QPixmap::fromImage(QImage(imagePath.toString())))
{
}

SourceImage::SourceImage(QStringView imagePath, const QPixmap &pixmap)
    : m_fullPath(imagePath.toString())
    , m_pixmap(pixmap)
{
}

QString SourceImage::baseName() const
{
    return QFileInfo(m_fullPath).baseName();
}

QString SourceImage::fullPath() const
{
    return m_fullPath;
}

QPixmap SourceImage::pixmap() const
{
    return m_pixmap;
}

bool SourceImage::isNull() const
{
    return m_pixmap.isNull();
}

QSize SourceImage::size() const
{
    return m_pixmap.size();
}

QRect SourceImage::rect() const
{
    return m_pixmap.rect();
}

int SourceImage::width() const
{
    return m_pixmap.width();
}

int SourceImage::height() const
{
    return m_pixmap.height();
}

bool SourceImage::saveImage() const
{
    QString fileName = QFileDialog::getSaveFileName(nullptr
                                                  , QStringLiteral("Save final image as...")
                                                  , QStringLiteral("untitled.png")
                                                  , QStringLiteral("Image (*.png)"));

    if (fileName.isEmpty())
        return true;

    return m_pixmap.save(fileName, "PNG");
}
