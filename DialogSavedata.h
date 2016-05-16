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
#ifndef DIALOGSAVEDATA_H
#define DIALOGSAVEDATA_H

#include <QDialog>
#include <QMap>
#include <QStringList>
#include <QThread>

namespace Ui {
class DialogSavedata;
}

class IGame;
class ISaveData;
class GameInfoLoader;

class QListWidgetItem;

class DialogSavedata : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSavedata(QWidget *parent = 0);
    ~DialogSavedata();

    IGame *loadGame() const;

private slots:
    void saveInfoLoaded(QString savedataName, ISaveData *gameInfo);

    void on_listWidget_itemSelectionChanged();
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButtonRemove_clicked();

private:
    enum ShownDataType {
        ShowAll,
        OnlySimpleSlide,
        OnlySwap,
        OnlyMineSweeper,
    };

    ISaveData *getSaveDataFromListItem(const QListWidgetItem *item) const;

    void onDataSelectionCleared();
    void onDataSelected(int row);

    void showAllSaveData();
    void showSelectedTypeData(ShownDataType shownDataType);

    void initComboBox();
    void initListWidget();

    Ui::DialogSavedata *ui;

    QThread loadThread;
    GameInfoLoader *infoLoader;
    QStringList savedataNames;

    QMap<QString, ShownDataType> gameTypeMap; // key = gameName
};

#endif // DIALOGSAVEDATA_H
