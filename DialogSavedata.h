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
#ifndef DIALOGSAVEDATA_H
#define DIALOGSAVEDATA_H

#include <QDialog>
#include <QStringList>
#include <QThread>

namespace Ui {
class DialogSavedata;
}

class SaveInfoLoader;

class DialogSavedata : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSavedata(QWidget *parent = 0);
    ~DialogSavedata();

private slots:
    void saveInfoLoaded(QString savedataName, QString gameName, QString imageBaseName);

    void on_listWidget_itemSelectionChanged();

private:
    enum SavedataList {
        ShowAll,
        OnlySimpleSlide,
        OnlySwap,
        OnlyMineSweeper,
    };

    void initComboBox();
    void initListWidget();

    Ui::DialogSavedata *ui;

    QThread loadThread;
    SaveInfoLoader *infoLoader;
    QStringList savedataNames;
};

#endif // DIALOGSAVEDATA_H
