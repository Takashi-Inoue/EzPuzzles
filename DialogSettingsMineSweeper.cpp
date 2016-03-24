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
#include "DialogSettingsMineSweeper.h"
#include "ui_DialogSettingsMineSweeper.h"

#include "mine/GameMineSweeper.h"

DialogSettingsMineSweeper::DialogSettingsMineSweeper(const QPixmap &sourcePixmap, bool showOkButton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsMineSweeper)
{
    ui->setupUi(this);

    ui->buttonBox->setVisible(showOkButton);

    ui->imageWidget->setPixmap(sourcePixmap);

    updateLabels();
    updateMineMax();

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabels()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMineMax()));}

DialogSettingsMineSweeper::~DialogSettingsMineSweeper()
{
    delete ui;
}

IGame *DialogSettingsMineSweeper::buildGame() const
{
    int cellSize = ui->horizontalSlider->value();
    QSize xy(xyCount());
    QSize fieldSize(xy * cellSize);

    return new MineSweeper::GameMineSweeper(ui->imageWidget->originalPixmap(), fieldSize, xy, ui->spinBox->value());
}

void DialogSettingsMineSweeper::updateLabels()
{
    ui->labelPixels->setText(QString("%1 x %1 pixels").arg(ui->horizontalSlider->value()));

    QSize xy = xyCount();
    ui->labelCells->setText(QString("%1 x %2 : %3 cells").arg(xy.width()).arg(xy.height()).arg(xy.width() * xy.height()));
}

void DialogSettingsMineSweeper::updateMineMax()
{
    QSize pixmapSize(ui->imageWidget->originalPixmap().size());

    int cellSize = ui->horizontalSlider->value();
    int cellnX = pixmapSize.width()  / cellSize;
    int cellnY = pixmapSize.height() / cellSize;

    ui->spinBox->setMaximum(cellnX * cellnY - 1);
}

QSize DialogSettingsMineSweeper::xyCount() const
{
    int cellSize = ui->horizontalSlider->value();
    QSize pixmapSize(ui->imageWidget->originalPixmap().size());

    return QSize(pixmapSize.width() / cellSize, pixmapSize.height() / cellSize);
}
