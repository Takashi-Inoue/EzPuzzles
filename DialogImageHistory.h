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
#ifndef DIALOGIMAGEHISTORY_H
#define DIALOGIMAGEHISTORY_H

#include <QDialog>
#include <QPixmap>
#include <QPushButton>
#include <QStringList>
#include <QShortcut>

#include "ThreadImageLoader.h"

namespace Ui {
class DialogImageHistory;
}

class DialogImageHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageHistory(QWidget *parent = nullptr);
    ~DialogImageHistory() override;

    QString selectedImagePath() const;

protected:
    void done(int result) override;

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_itemSelectionChanged();

private:
    static constexpr char m_geometryKey[] = "DialogImageHistory_Geometry";

    void removeHistory(int index);
    void saveImageHistory();

    Ui::DialogImageHistory *ui;

    int m_defaultThumbSize = -1;
};

#endif // DIALOGIMAGEHISTORY_H
