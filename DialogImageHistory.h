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
#ifndef DIALOGIMAGEHISTORY_H
#define DIALOGIMAGEHISTORY_H

#include <QDialog>
#include <QPixmap>
#include <QPushButton>
#include <QStringList>
#include <QShortcut>
#include <QThread>

#include "ImageLoader.h"

namespace Ui {
class DialogImageHistory;
}

class DialogImageHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageHistory(QWidget *parent = 0);
    ~DialogImageHistory();

    QString selectedImagePath() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void done(int result) override;

private slots:
    void imageLoaded(const QString &imagePath, const QPixmap &pixmap);
    void removeImagePath(const QString &imagePath);
    void removeSelected();
    void onRemoveButtonPressed();

    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_itemSelectionChanged();

private:
    void removeHistory(int index);

    void setupRemoveButton();
    void drawIconEdge(int edgeWidth, QRect iconImageRect);
    QSize iconImageSize(int edgeWidth) const;
    QPoint iconImageTopLeft(const QSize &iconPixmapSize) const;

    Ui::DialogImageHistory *ui;

    QThread loadThread;
    ImageLoader imageLoader;

    QPixmap iconBase;
    QStringList imagePaths;

    QPushButton buttonRemove;
    QShortcut shortcutRemove;

    bool isHistoryChanged;
};

#endif // DIALOGIMAGEHISTORY_H
