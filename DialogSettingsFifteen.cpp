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
#include "DialogSettingsFifteen.h"
#include "ui_DialogSettingsFifteen.h"

#include "SelectCellGrid.h"
#include "SourceImage.h"
#include "fifteen/GameSimpleSlide.h"
#include "fifteen/GameSimpleSwap.h"

#include <QDebug>

DialogSettingsFifteen::DialogSettingsFifteen(const SourceImage &sourceImage, bool showOkButton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsFifteen),
    grid(new SelectCellGrid(2, 2)),
    sourceImage(sourceImage)
{
    ui->setupUi(this);

    Q_ASSERT(!sourceImage.isNull());

    ui->buttonBox->setVisible(showOkButton);

    ui->comboBoxGameType->addItem("Simple Slide", idFifteen);
    ui->comboBoxGameType->addItem("Simple Swap", idSwap);

    ui->imageWidget->setPixmap(sourceImage.pixmap);
    ui->imageWidget->addSubWidget(grid);

    connect(ui->hSliderSplitX, SIGNAL(valueChanged(int)), ui->spinBoxSplitX, SLOT(setValue(int)));
    connect(ui->hSliderSplitY, SIGNAL(valueChanged(int)), ui->spinBoxSplitY, SLOT(setValue(int)));
    connect(ui->spinBoxSplitX, SIGNAL(valueChanged(int)), ui->hSliderSplitX, SLOT(setValue(int)));
    connect(ui->spinBoxSplitY, SIGNAL(valueChanged(int)), ui->hSliderSplitY, SLOT(setValue(int)));
    connect(ui->hSliderSplitX, SIGNAL(valueChanged(int)), this, SLOT(udpateGrid()));
    connect(ui->hSliderSplitY, SIGNAL(valueChanged(int)), this, SLOT(udpateGrid()));
    connect(ui->radioButtonBlankRandom,    SIGNAL(clicked()), this, SLOT(onChangeBlankSetting()));
    connect(ui->radioButtonBlankSpecified, SIGNAL(clicked()), this, SLOT(onChangeBlankSetting()));
}

DialogSettingsFifteen::~DialogSettingsFifteen()
{
    delete ui;
}

IGame *DialogSettingsFifteen::buildGame() const
{
    if (ui->comboBoxGameType->currentData() == idFifteen)
        return buildSimpleSlide();

    if (ui->comboBoxGameType->currentData() == idSwap)
        return buildSimpleSwap();

    Q_ASSERT(false);

    return nullptr;
}

void DialogSettingsFifteen::udpateGrid()
{
    grid->setCellCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());
}

void DialogSettingsFifteen::onChangeBlankSetting()
{
    grid->setRandomSelect(ui->radioButtonBlankRandom->isChecked());
}

IGame *DialogSettingsFifteen::buildSimpleSlide() const
{
    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition defaultBlank;

    ui->radioButtonBlankRandom->isChecked() ? defaultBlank.randomSelect(xyCount)
                                            : defaultBlank.select(grid->selectedCellPos());

    return new Fifteen::GameSimpleSlide(sourceImage, xyCount, defaultBlank);
}

IGame *DialogSettingsFifteen::buildSimpleSwap() const
{
    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition swapTarget;

    ui->radioButtonBlankRandom->isChecked() ? swapTarget.randomSelect(xyCount)
                                            : swapTarget.select(grid->selectedCellPos());

    return new Fifteen::GameSimpleSwap(sourceImage, xyCount, swapTarget);
}
