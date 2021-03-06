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

#include "DialogStartGame.h"
#include "ui_DialogStartGame.h"

#include "Application.h"
#include "ImageWidget/ImageWidget.h"

#include <QSettings>

DialogStartGame::DialogStartGame(QWidget *parent)
    : DialogStartGame(QString(), parent)
{
}

DialogStartGame::DialogStartGame(QStringView imageFullPathName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStartGame)
{
    ui->setupUi(this);

    QVariant savedGeometry = QSettings(Application::iniFilePathName(), QSettings::IniFormat)
                             .value(m_geometryKey);

    if (savedGeometry.isValid())
        restoreGeometry(savedGeometry.toByteArray());

    auto imageWidget = findChild<ImageWidget *>();

    ui->formSettingsMineSweeper->setImageWidget(imageWidget);
    ui->formSettingsLikeFifteen->setImageWidget(imageWidget);

    imageWidget->setImage(imageFullPathName);
}

DialogStartGame::~DialogStartGame()
{
    delete ui;
}

QSharedPointer<IGame> DialogStartGame::buildGame() const
{
    QWidget *widget = ui->tabWidget->currentWidget();

    if (widget == ui->formSettingsLikeFifteen)
        return ui->formSettingsLikeFifteen->buildGame();

    if (widget == ui->formSettingsMineSweeper)
        return ui->formSettingsMineSweeper->buildGame();

    return nullptr;
}

void DialogStartGame::done(int result)
{
    QSettings(Application::iniFilePathName(), QSettings::IniFormat)
             .setValue(m_geometryKey, saveGeometry());

    QDialog::done(result);
}
