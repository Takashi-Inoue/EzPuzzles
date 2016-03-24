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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DialogGameStart.h"
#include "FormFinalImage.h"
#include "IGame.h"
#include "GameWidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QImage>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameWidget(new GameWidget())
{
    ui->setupUi(this);

    gameWidget->resize(1, 1);
    ui->scrollArea->setWidget(gameWidget);

    QFont font(ui->dockWidget->font());
    font.setBold(true);
    ui->dockWidget->setFont(font);

    ui->action_Final_image->setChecked(ui->dockWidget->isVisible());

    connect(ui->menu_Window, SIGNAL(aboutToShow()), this, SLOT(onMenuWindowAboutToShow()));
    connect(ui->dockWidget, SIGNAL(visibilityChanged(bool)), ui->action_Final_image, SLOT(setChecked(bool)));
}

MainWindow::~MainWindow()
{
    delete gameWidget;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::onMenuWindowAboutToShow()
{
    ui->action_Final_image->setEnabled(ui->dockWidget->isHidden());
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image..."), "", tr("Images (*.png *.bmp *.jpg *.jpeg)"));

    if (fileName.isNull())
        return;

    QImage image(fileName);

    if (image.isNull())
        return;

    startNewGame(QPixmap::fromImage(image));
}

void MainWindow::on_actionNew_Game_Current_image_triggered()
{
    startNewGame(game->pixmap());
}

void MainWindow::on_action_Final_image_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::createFinalImageWidget()
{
    Q_CHECK_PTR(game.get());
    Q_ASSERT(finalImageWidget == nullptr);

    finalImageWidget = std::make_shared<FormFinalImage>(game);

    ui->dockWidget->setWidget(finalImageWidget.get());

    connect(finalImageWidget.get(), SIGNAL(windowTitleChanged(QString)), ui->dockWidget, SLOT(setWindowTitle(QString)));
}

void MainWindow::startNewGame(QPixmap sourcePixmap)
{
    DialogGameStart dialog(sourcePixmap, this);

    if (dialog.exec() == QDialog::Rejected)
        return;

    if (game != nullptr)
        game->disconnect();

    game.reset(dialog.buildGame());

    if (finalImageWidget == nullptr)
        createFinalImageWidget();

    finalImageWidget->setGame(game);
    finalImageWidget->repaint();

    connect(game.get(), SIGNAL(screenUpdated()), finalImageWidget.get(), SLOT(repaint()));

    gameWidget->resize(game->maxFieldSize());
    gameWidget->setGame(game);

    gameWidget->repaint();

    ui->actionNew_Game_Current_image->setEnabled(true);
}
