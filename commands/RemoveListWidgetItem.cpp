/*
 * Copyright 2021 Takashi Inoue
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

#include "RemoveListWidgetItem.h"

#include <QListWidget>

RemoveListWidgetItem::RemoveListWidgetItem(QListWidget *listWidget, int row)
    : m_listWidget(listWidget)
    , m_row(row)
{
    Q_CHECK_PTR(listWidget);
}

RemoveListWidgetItem::~RemoveListWidgetItem()
{
    delete m_listWidgetItem;
}

void RemoveListWidgetItem::redo()
{
    m_listWidgetItem = m_listWidget->takeItem(m_row);
}

void RemoveListWidgetItem::undo()
{
    m_listWidget->insertItem(m_row, m_listWidgetItem);
}
