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

#include "EzPuzzles.h"
#include "AbstractSaveData.h"

#include <QDialog>
#include <QMap>
#include <QStringList>

namespace Ui {
class DialogSavedata;
}

class IGame;
class ThreadGameInfoLoader;

class QListWidgetItem;

class DialogSavedata : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSavedata(QWidget *parent = nullptr);
    ~DialogSavedata() override;

    QSharedPointer<IGame> loadGame() const;

private slots:
    void onSaveInfoLoaded(QString savedataName, QSharedPointer<AbstractSaveData> gameInfo);
    void setUIState();
    void onDataSelected(int row);

    void on_comboBox_currentIndexChanged(int index);
    void on_pushButtonRemove_clicked();

private:
    enum class ShownData : int {
        all,
        typeSlide,
        typeSwap,
        simpleSlide,
        simpleSwap,
        mineSweeper,
    };

    QString saveDataPathName(int row) const;
    QString thumbnailPathName(int row) const;

    QSharedPointer<AbstractSaveData> saveDataFromListItem(const QListWidgetItem *item) const;

    void showAllSaveData();
    void showSpecifiedTypeData(ShownData shownDataType);

    void initComboBox();
    void initListWidget();

    Ui::DialogSavedata *ui;

    ThreadGameInfoLoader *m_threadInfoLoader;
    QStringList m_savedataNames;
};

#endif // DIALOGSAVEDATA_H
