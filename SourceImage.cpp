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

SourceImage::SourceImage(const QString &imagePath) :
    fullPath(imagePath),
    pixmap(QPixmap::fromImage(QImage(imagePath)))
{
}

SourceImage::SourceImage(const QString &imagePath, const QPixmap &pixmap) :
    fullPath(imagePath),
    pixmap(pixmap)
{
}

bool SourceImage::isNull() const
{
    return pixmap.isNull();
}

QString SourceImage::baseName() const
{
    return QFileInfo(fullPath).baseName();
}

QSize SourceImage::size() const
{
    return pixmap.size();
}

QRect SourceImage::rect() const
{
    return pixmap.rect();
}

int SourceImage::width() const
{
    return pixmap.width();
}

int SourceImage::height() const
{
    return pixmap.height();
}

bool SourceImage::saveImage() const
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save final image as..."), "untitled.png", "Image (*.png)");

    if (fileName.isEmpty())
        return true;

    return pixmap.save(fileName, "PNG");
}
