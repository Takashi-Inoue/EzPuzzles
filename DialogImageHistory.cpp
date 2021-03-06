﻿/*
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

#include "Application.h"
#include "ImageHistory.h"

#include <QSettings>
#include <QDebug>

DialogImageHistory::DialogImageHistory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogImageHistory)
{
    ui->setupUi(this);

    ui->labelDescription->setText(QStringLiteral("Shortcut: [Remove] %1, [Undo] %2, [Redo] %3")
                                 .arg(QKeySequence(QKeySequence::Delete).toString()
                                    , QKeySequence(QKeySequence::Undo).toString()
                                    , QKeySequence(QKeySequence::Redo).toString()));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setToolTip(
                QStringLiteral("Start game using selected image."));

    ui->buttonBox->button(QDialogButtonBox::Close)->setToolTip(
                QStringLiteral("Close dialog. \nChanges to the image history will be retained."));

    for (int size : ImageHistory::thumbnailSizeList())
        ui->comboBox->addItem(QString("%1").arg(size));

    QVariant savedGeometry = QSettings(Application::iniFilePathName(), QSettings::IniFormat)
                             .value(m_geometryKey);

    if (savedGeometry.isValid())
        restoreGeometry(savedGeometry.toByteArray());

    ImageHistory history;

    history.load();

    m_defaultThumbSize = history.thumbnailSize();

    ui->comboBox->setCurrentText(QString("%1").arg(m_defaultThumbSize));

    ui->listWidget->setIconSize(QSize(m_defaultThumbSize, m_defaultThumbSize));
    ui->listWidget->addPaths(history.stringList());
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
    ui->listWidget->stopLoading();

    if (ui->listWidget->isHistoryChanged()
     || ui->listWidget->iconSize().width() != m_defaultThumbSize)
    {
        saveImageHistory();
    }

    QSettings(Application::iniFilePathName(), QSettings::IniFormat)
             .setValue(m_geometryKey, saveGeometry());

    QDialog::done(result);
}

void DialogImageHistory::on_comboBox_currentTextChanged(const QString &arg1)
{
    int size = arg1.toInt();
    ui->listWidget->setIconSize(QSize(size, size));
}

void DialogImageHistory::on_listWidget_doubleClicked(const QModelIndex &/*index*/)
{
    accept();
}

void DialogImageHistory::on_listWidget_itemSelectionChanged()
{
    bool hasSelection = ui->listWidget->selectionModel()->hasSelection();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasSelection);
}

void DialogImageHistory::saveImageHistory()
{
    ImageHistory history;

    history.setThumbnailSize(ui->comboBox->currentText().toInt());
    history.addStrings(ui->listWidget->allImagePathNames(), false);
    history.save();
}
