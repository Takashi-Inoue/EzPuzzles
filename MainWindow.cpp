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
#include <QThread>
#include <QDebug>

QThread garbageThread;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(nullptr),
    gameWidget(new GameWidget())
{
    ui->setupUi(this);

    garbageCollector.moveToThread(&garbageThread);
    garbageThread.start();

    gameWidget->resize(1, 1);
    ui->scrollArea->setWidget(gameWidget);

    QFont font(ui->dockWidget->font());
    font.setBold(true);
    ui->dockWidget->setFont(font);

    ui->action_Final_image->setChecked(ui->dockWidget->isVisible());

    connect(ui->dockWidget, SIGNAL(visibilityChanged(bool)), ui->action_Final_image, SLOT(setChecked(bool)));
}

MainWindow::~MainWindow()
{
    garbageCollector.stop();
    garbageCollector.wait();

    garbageThread.quit();

    delete gameWidget;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image..."), "", tr("Images (*.png *.bmp *.jpg *.jpeg)"));

    if (fileName.isNull())
        return;

    QImage image(fileName);

    if (image.isNull())
        return;

    auto newGame = createNewGame(QPixmap::fromImage(image));

    if (newGame != nullptr)
        startNewGame(newGame);
}

void MainWindow::on_actionNew_Game_Current_image_triggered()
{
    auto newGame = createNewGame(game->pixmap());

    if (newGame != nullptr)
        startNewGame(newGame);
}

void MainWindow::on_action_Final_image_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_action_Restart_triggered()
{

}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::createFinalImageWidget(IGame *game)
{
    Q_CHECK_PTR(game);
    Q_ASSERT(finalImageWidget == nullptr);

    finalImageWidget = std::make_shared<FormFinalImage>(game);

    ui->dockWidget->setWidget(finalImageWidget.get());

    connect(finalImageWidget.get(), SIGNAL(windowTitleChanged(QString)), ui->dockWidget, SLOT(setWindowTitle(QString)));
}

IGame *MainWindow::createNewGame(const QPixmap &sourcePixmap)
{
    DialogGameStart dialog(sourcePixmap, this);

    if (dialog.exec() == QDialog::Rejected)
        return nullptr;

    return dialog.buildGame();
}

void MainWindow::startNewGame(IGame *newGame)
{
    Q_CHECK_PTR(newGame);

    if (game != nullptr)
        game->disconnect();

    if (finalImageWidget == nullptr)
        createFinalImageWidget(newGame);

    finalImageWidget->setGame(newGame);
    finalImageWidget->repaint();

    connect(newGame, SIGNAL(informationUpdated()), finalImageWidget.get(), SLOT(repaint()));

    gameWidget->resize(newGame->maxFieldSize());
    gameWidget->setGame(newGame);

    gameWidget->repaint();

    ui->actionNew_Game_Current_image->setEnabled(true);
    ui->action_Restart->setEnabled(true);

    auto oldGame = game;
    game = newGame;

//    qDebug() << "MainThreadID :" << this->thread()->currentThreadId();

    if (oldGame != nullptr) {
        garbageCollector.push(oldGame);
        garbageCollector.start();
    }
}

