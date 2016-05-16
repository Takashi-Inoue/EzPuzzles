﻿/*
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

#include "ImageLoader.h"
#include <QImage>
#include <QPixmap>
#include <QWriteLocker>
#include <QDebug>

void ImageLoader::append(const QString &fileName)
{
    QWriteLocker locker(&lock);

    if (fileNames.contains(fileName))
        return;

    fileNames << fileName;
}

QString ImageLoader::className() const
{
    return "ImageLoader";
}

void ImageLoader::execImpl()
{
    QWriteLocker locker(&lock);

    while (!fileNames.isEmpty()) {
        const auto &fileName = fileNames.takeFirst();

        locker.unlock();

        if (isStopped()) {
            qInfo() << "--ABORTED loading images : class" << className();
            return;
        }

        QImage image(fileName); // QPixmap で読み込むと警告or読めない場合があるので

        if (!image.isNull()) {
            QPixmap pixmap(QPixmap::fromImage(image));

            if (!pixmap.isNull())
                emit loaded(fileName, pixmap);
            else
                emit failedToLoad(fileName);
        } else {
            emit failedToLoad(fileName);
        }

        locker.relock();
    }
}

