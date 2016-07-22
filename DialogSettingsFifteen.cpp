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

#include "GridSplitter.h"
#include "SourceImage.h"
#include "fifteen/GameSimpleSlide.h"
#include "fifteen/GameSimpleSwap.h"

#include <QDebug>

DialogSettingsFifteen::DialogSettingsFifteen(const SourceImage &sourceImage, bool showOkButton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsFifteen),
    sourceImage(sourceImage)
{
    ui->setupUi(this);

    Q_ASSERT(!sourceImage.isNull());

    ui->buttonBox->setVisible(showOkButton);

    ui->comboBoxGameType->addItem("Slide mode", idFifteen);
    ui->comboBoxGameType->addItem("Swap mode", idSwap);

    ui->imageWidget->setPixmap(sourceImage.pixmap);
    ui->imageWidget->addSubWidget(new GridSplitter(1, 1));

    connect(ui->hSliderSplitX, SIGNAL(valueChanged(int)), ui->spinBoxSplitX, SLOT(setValue(int)));
    connect(ui->hSliderSplitY, SIGNAL(valueChanged(int)), ui->spinBoxSplitY, SLOT(setValue(int)));
    connect(ui->spinBoxSplitX, SIGNAL(valueChanged(int)), ui->hSliderSplitX, SLOT(setValue(int)));
    connect(ui->spinBoxSplitY, SIGNAL(valueChanged(int)), ui->hSliderSplitY, SLOT(setValue(int)));
    connect(ui->hSliderSplitX, SIGNAL(valueChanged(int)), this, SLOT(updateSplitPainter()));
    connect(ui->hSliderSplitY, SIGNAL(valueChanged(int)), this, SLOT(updateSplitPainter()));
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

void DialogSettingsFifteen::updateSplitPainter() const
{
    GridSplitter *painter = new GridSplitter(ui->hSliderSplitX->value() - 1, ui->hSliderSplitY->value() - 1);
    ui->imageWidget->replaceSubWidget(0, painter);
    ui->imageWidget->update();
}

IGame *DialogSettingsFifteen::buildSimpleSlide() const
{
    QSize numXY(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());
    QPoint blankPos(numXY.width() - 1, numXY.height() - 1);

    return new Fifteen::GameSimpleSlide(sourceImage, numXY, blankPos);
}

IGame *DialogSettingsFifteen::buildSimpleSwap() const
{
    QSize numXY(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());
    QPoint swapTargetPos(0, 0);

    return new Fifteen::GameSimpleSwap(sourceImage, numXY, swapTargetPos);
}
