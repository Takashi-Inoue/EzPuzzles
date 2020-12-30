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

#ifndef LISTWIDGETIMAGEHISTORY_H
#define LISTWIDGETIMAGEHISTORY_H

#include "ListWidgetImages.h"
#include <QAction>
#include <QPushButton>
#include <QUndoStack>

class ListWidgetImageHistory : public ListWidgetImages
{
    Q_OBJECT
public:
    ListWidgetImageHistory(QWidget *parent = nullptr);

    bool isHistoryChanged() const;

protected:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onItemEntered(QListWidgetItem *item);
    void onActionRemoveTriggered();
    void onButtonRemoveClicked();
    void onSliderValueChanged();

private:
    QPoint buttonPosOnItem(QListWidgetItem *item) const;
    void removeItem(int row);

    QAction m_actionRemove;
    QPushButton m_buttonRemove;
    QUndoStack m_undoStack;
};

#endif // LISTWIDGETIMAGEHISTORY_H
