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

#include "SplitPainter.h"
#include "GameFifteen.h"
#include "GameSwap.h"

#include <QDebug>

DialogSettingsFifteen::DialogSettingsFifteen(const QPixmap &pixmap, bool showOkButton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingsFifteen)
{
    ui->setupUi(this);

    Q_ASSERT(!pixmap.isNull());

    ui->buttonBox->setVisible(showOkButton);

    ui->comboBoxGameType->addItem("Slide", idFifteen);
    ui->comboBoxGameType->addItem("Swap for TopLeft", idSwap);

    ui->imageWidget->setPixmap(pixmap);
    ui->imageWidget->addSubWidget(new SplitPainter(1, 1));

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
        return buildGameFifteen();

    if (ui->comboBoxGameType->currentData() == idSwap)
        return buildGameSwap();

    Q_ASSERT(false);

    return nullptr;
}

void DialogSettingsFifteen::updateSplitPainter() const
{
    SplitPainter *painter = new SplitPainter(ui->hSliderSplitX->value() - 1, ui->hSliderSplitY->value() - 1);
    ui->imageWidget->replaceSubWidget(0, painter);
    ui->imageWidget->update();
}

IGame *DialogSettingsFifteen::buildGameFifteen() const
{
    return new GameFifteen(ui->imageWidget->originalPixmap(), QSize(ui->hSliderSplitX->value(), ui->hSliderSplitY->value()));
}

IGame *DialogSettingsFifteen::buildGameSwap() const
{
    return new GameSwap(ui->imageWidget->originalPixmap(), QSize(ui->hSliderSplitX->value(), ui->hSliderSplitY->value()));
}
