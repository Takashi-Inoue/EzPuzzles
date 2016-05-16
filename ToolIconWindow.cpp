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
#include "ToolIconWindow.h"

#include <QVBoxLayout>

ToolIconWindow::ToolIconWindow(QWidget *parent) :
    widget(parent)
{
    widget.setWindowFlags(Qt::Popup);
    widget.setWindowModality(Qt::NonModal);
    widget.setGeometry(0, 0, 1, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setMargin(2);

    widget.setLayout(layout);
}

QPushButton *ToolIconWindow::addIcon(const QIcon &icon, const QString &toolTip)
{
    QPushButton *button = new QPushButton(icon, "");
    button->setMinimumSize(0, 0);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    button->setIconSize(QSize(32, 32));
    button->setToolTip(toolTip);

    widget.layout()->addWidget(button);

    return button;
}

void ToolIconWindow::show(const QPoint &pos)
{
    widget.move(pos);
    widget.show();
}

void ToolIconWindow::close()
{
    widget.close();
}

void ToolIconWindow::setChildEnabled(const QString &objectName, bool isEnable)
{
    auto child = widget.findChild<QWidget *>(objectName);

    if (child != nullptr)
        child->setEnabled(isEnable);
}
