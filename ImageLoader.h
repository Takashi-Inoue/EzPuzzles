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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "ThreadOperation.h"
#include <QReadWriteLock>
#include <QStringList>
#include <QVariant>

class ImageLoader : public ThreadOperation
{
    Q_OBJECT
public:
    ImageLoader() = default;
    ~ImageLoader() = default;

    void append(const QString &fileName);

public slots:

signals:
    void loaded(const QString &fileName, const QPixmap &pixmap);
    void failedToLoad(const QString &fileName);

protected:
    QString className() const override;
    void execImpl() override;

private:
    QReadWriteLock lock;

    QStringList fileNames;
};

#endif // IMAGELOADER_H
