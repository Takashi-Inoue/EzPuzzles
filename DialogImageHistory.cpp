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
#include "DialogImageHistory.h"
#include "ui_DialogImageHistory.h"

#include "EzPuzzles.h"
#include "StringListHistory.h"

#include <QDebug>

DialogImageHistory::DialogImageHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageHistory),
    isHistoryChanged(false)
{
    ui->setupUi(this);

    ui->listWidget->viewport()->setMouseTracking(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Start game"));
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("Apply changes"));

    StringListHistory history;
    history.load(EzPuzzles::imageHistoryPath());

    imagePaths = history.stringList();

    if (imagePaths.isEmpty())
        return;

    ui->listWidget->addPath(imagePaths);
}

DialogImageHistory::~DialogImageHistory()
{
    delete ui;
}

QString DialogImageHistory::selectedImagePath() const
{
    return ui->listWidget->currentImagePathName();
}

void DialogImageHistory::done(int result)
{
    if (isHistoryChanged && result == QDialog::Accepted)
        saveImageHistory();

    QDialog::done(result);
}

void DialogImageHistory::removeImagePath(QString imagePath)
{
    int index = -1;

    while ((index = imagePaths.indexOf(imagePath)) != -1)
        removeHistory(index);
}

void DialogImageHistory::removeSelected()
{
    int row = ui->listWidget->currentRow();

    if (row == -1)
        return;

    removeHistory(row);
}

void DialogImageHistory::on_listWidget_doubleClicked(const QModelIndex &/*index*/)
{
    accept();
}

void DialogImageHistory::on_listWidget_itemSelectionChanged()
{
    bool hasSelection = ui->listWidget->selectionModel()->hasSelection();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isHistoryChanged | hasSelection);
}

void DialogImageHistory::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button) != QDialogButtonBox::ApplyRole)
        return;

    saveImageHistory();
}

void DialogImageHistory::removeHistory(int index)
{
    Q_ASSERT(index >= 0 && index < imagePaths.size());

    imagePaths.removeAt(index);
    ui->listWidget->model()->removeRow(index);

    isHistoryChanged = true;

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void DialogImageHistory::saveImageHistory()
{
    StringListHistory history;

    history.addStrings(imagePaths, false);
    history.save(EzPuzzles::imageHistoryPath());

    isHistoryChanged = false;

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}
