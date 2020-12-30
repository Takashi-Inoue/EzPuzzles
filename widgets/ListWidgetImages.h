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

#ifndef LISTWIDGETIMAGES_H
#define LISTWIDGETIMAGES_H

#include <QListWidget>

#include "ThreadImageLoader.h"

class ListWidgetImages : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidgetImages(QWidget *parent = nullptr);
    ~ListWidgetImages() override;

    void addPath(QStringView imagePathName);
    void addPaths(const QStringList &imagePathNames);

    QString currentImagePathName() const;
    QStringList allImagePathNames() const;

protected:
    ThreadImageLoader *m_imageLoader;

private slots:
    void onIconSizeChanged();
    void onImageLoaded(QString imagePathName, QVariant data, QPixmap pixmap);
    void onLoadingFailed(QString imagePathName, QVariant data);

private:
    static constexpr int PathNameRole = Qt::UserRole;

    void createNewItem(QStringView imagePathName);
    void createNewItem(const QStringList &imagePathNames);
};

#endif // LISTWIDGETIMAGES_H
