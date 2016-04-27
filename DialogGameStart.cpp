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
#include "DialogGameStart.h"
#include "ui_DialogGameStart.h"

#include "DialogSettingsFifteen.h"
#include "DialogSettingsMineSweeper.h"

DialogGameStart::DialogGameStart(const SourceImage &sourceImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGameStart)
{
    ui->setupUi(this);

    ui->tabLikeFifteen->setLayout(new QVBoxLayout());
    ui->tabMineSweeper->setLayout(new QVBoxLayout());

    ui->tabLikeFifteen->layout()->addWidget(new DialogSettingsFifteen(sourceImage, false));
    ui->tabMineSweeper->layout()->addWidget(new DialogSettingsMineSweeper(sourceImage, false));
}

DialogGameStart::~DialogGameStart()
{
    delete ui;
}

IGame *DialogGameStart::buildGame() const
{
    IGameBuilder *builder = nullptr;

    if (ui->tabWidget->currentWidget() == ui->tabLikeFifteen)
        builder = static_cast<IGameBuilder *>(ui->tabLikeFifteen->findChild<DialogSettingsFifteen *>());

    if (ui->tabWidget->currentWidget() == ui->tabMineSweeper)
        builder = static_cast<IGameBuilder *>(ui->tabMineSweeper->findChild<DialogSettingsMineSweeper *>());

    if (builder)
        return builder->buildGame();

    Q_ASSERT(false);

    return nullptr;
}
