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

#include "ListWidgetImageHistory.h"
#include "commands/RemoveListWidgetItem.h"

#include <QMouseEvent>
#include <QDebug>

ListWidgetImageHistory::ListWidgetImageHistory(QWidget *parent)
    : ListWidgetImages(parent)
    , m_actionRemove(QIcon(":/icons/remove"), "Remove", this)
    , m_buttonRemove(this)
    , m_undoStack(this)
{
    setMouseTracking(true);

    m_actionRemove.setEnabled(false);
    addAction(&m_actionRemove);
    addAction(m_undoStack.createUndoAction(this));
    addAction(m_undoStack.createRedoAction(this));

    m_buttonRemove.close();
    m_buttonRemove.resize(18, 18);
    m_buttonRemove.setIconSize(QSize(10, 10));
    m_buttonRemove.setIcon(QIcon(":/icons/remove"));
    m_buttonRemove.setToolTip(QStringLiteral("Remove this"));

    connect(&m_actionRemove, &QAction::triggered, this, &ListWidgetImageHistory::onActionRemoveTriggered);
    connect(&m_buttonRemove, &QPushButton::clicked, this, &ListWidgetImageHistory::onButtonRemoveClicked);
    connect(this, &QListWidget::itemEntered, this, &ListWidgetImageHistory::onItemEntered);
}

void ListWidgetImageHistory::selectionChanged(const QItemSelection &selected
                                            , const QItemSelection &/*deselected*/)
{
    m_actionRemove.setEnabled(!selected.isEmpty());
}

void ListWidgetImageHistory::mouseMoveEvent(QMouseEvent *event)
{
    ListWidgetImages::mouseMoveEvent(event);

    if (m_buttonRemove.isVisible() && itemAt(event->pos()) == nullptr)
        m_buttonRemove.close();
}

void ListWidgetImageHistory::wheelEvent(QWheelEvent *e)
{
    ListWidgetImages::wheelEvent(e);

    if (m_buttonRemove.isVisible() && itemAt(e->position().toPoint()) == nullptr)
        m_buttonRemove.close();
}

void ListWidgetImageHistory::onItemEntered(QListWidgetItem *item)
{
    QPoint buttonTL = visualItemRect(item).topRight() + QPoint(-17, 3);

    m_buttonRemove.move(buttonTL);
    m_buttonRemove.show();
}

void ListWidgetImageHistory::onActionRemoveTriggered()
{
    removeItem(currentRow());
}

void ListWidgetImageHistory::onButtonRemoveClicked()
{
    if (!indexAt(m_buttonRemove.pos()).isValid())
        m_buttonRemove.close();

    removeItem(indexAt(m_buttonRemove.pos()).row());
}

void ListWidgetImageHistory::removeItem(int row)
{
    m_undoStack.push(new RemoveListWidgetItem(this, row));
}
