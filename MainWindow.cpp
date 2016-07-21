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
#include "DialogImageHistory.h"
#include "DialogSavedata.h"
#include "FormFinalImage.h"

#include "IGame.h"
#include "GameWidget.h"

#include "EzPuzzles.h"
#include "StringListHistory.h"
#include "SourceImage.h"

#include <QFileDialog>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

QThread garbageThread;
QThread timerThread;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    newGameToolBox(this),
    diskToolBox(this),
    game(nullptr),
    gameWidget(new GameWidget())
{
    qDebug() << "MainThread :" << thread()->currentThreadId();

    ui->setupUi(this);

    initToolBoxies();

    garbageCollector.moveToThread(&garbageThread);
    garbageThread.start();

    gameWidget->resize(1, 1);
    ui->scrollArea->setWidget(gameWidget);

    QFont font(ui->dockWidget->font());
    font.setBold(true);
    ui->dockWidget->setFont(font);

    ui->action_Final_image->setChecked(ui->dockWidget->isVisible());

    connect(ui->dockWidget, SIGNAL(visibilityChanged(bool)), ui->action_Final_image, SLOT(setChecked(bool)));

    frameTimer.moveToThread(&timerThread);

    connect(&frameTimer, SIGNAL(tick(QMutex*,QWaitCondition*)), this, SLOT(onTickFrameTimer(QMutex*,QWaitCondition*)));

    timerThread.start();
    frameTimer.start();
}

MainWindow::~MainWindow()
{
    garbageCollector.stop();
    garbageCollector.wait();

    garbageThread.quit();
    garbageThread.wait();

    frameTimer.stop();
    frameTimer.wait();

    timerThread.quit();
    timerThread.wait();

    delete gameWidget;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::on_action_Open_triggered()
{
    newGameToolBox.show(QCursor::pos());
}

void MainWindow::on_actionNew_Game_Current_image_triggered()
{
    auto newGame = createNewGame(game->sourceImage());

    if (newGame != nullptr)
        startNewGame(newGame);
}

void MainWindow::on_actionSave_Load_triggered()
{
    diskToolBox.show(QCursor::pos());
}

void MainWindow::on_action_Restart_triggered()
{
    startNewGame(game->cloneAsNewGame());
}

void MainWindow::on_action_Final_image_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::startGameWithNewImage()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open image..."), "", tr("Images (*.png *.bmp *.jpg *.jpeg)"));

    SourceImage sourceImage(imagePath);

    if (sourceImage.isNull())
        return;

    auto newGame = createNewGame(sourceImage);

    if (newGame == nullptr)
        return;

    startNewGame(newGame);
    updateImageHistory(imagePath);
}

void MainWindow::startGameFromImageHistory()
{
    DialogImageHistory dialog(this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QString imagePath = dialog.selectedImagePath();

    SourceImage sourceImage(imagePath);

    if (sourceImage.isNull())
        return;

    auto newGame = createNewGame(sourceImage);

    if (newGame == nullptr)
        return;

    startNewGame(newGame);
    updateImageHistory(imagePath);
}

void MainWindow::saveGame()
{
    diskToolBox.close();

    if (game == nullptr)
        return;

    EzPuzzles::createSaveDirPath();
    game->save(EzPuzzles::saveDirPath(), EzPuzzles::screenshotSize());
}

void MainWindow::loadGame()
{
    DialogSavedata dialog(this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    auto game = dialog.loadGame();

    if (game == nullptr)
        return;

    startNewGame(game);
    updateImageHistory(game->sourceImage().fullPath);
}

void MainWindow::onTickFrameTimer(QMutex *mutex, QWaitCondition *wait)
{
    mutex->lock();

//    qDebug() << "onTickFrameTimer lock";

    if (game != nullptr)
        game->onTickFrame();

    gameWidget->repaint();

//    qDebug() << "onTickFrameTimer wake";
    wait->wakeAll();
    mutex->unlock();
}

void MainWindow::initToolBoxies()
{
    auto buttonOpenImage = newGameToolBox.addIcon(QIcon(":/ico/openImage"), "Open new image");
    auto buttonHistory = newGameToolBox.addIcon(QIcon(":/ico/historyImage"), "Select an image from history");

    connect(buttonOpenImage, SIGNAL(clicked(bool)), this, SLOT(startGameWithNewImage()));
    connect(buttonHistory,   SIGNAL(clicked(bool)), this, SLOT(startGameFromImageHistory()));

    auto buttonSaveGame = diskToolBox.addIcon(QIcon(":/ico/save"), "Save");
    auto buttonLoadGame = diskToolBox.addIcon(QIcon(":/ico/load"), "Load");

    connect(buttonSaveGame, SIGNAL(clicked(bool)), this, SLOT(saveGame()));
    connect(buttonLoadGame, SIGNAL(clicked(bool)), this, SLOT(loadGame()));

    buttonSaveGame->setObjectName("pushButtonSaveGame");
    buttonSaveGame->setEnabled(false);
}

void MainWindow::updateImageHistory(const QString &lastImagePath) const
{
    StringListHistory history;

    history.load(EzPuzzles::imageHistoryPath());
    history.addString(lastImagePath);
    history.save(EzPuzzles::imageHistoryPath());
}

void MainWindow::createFinalImageWidget(IGame *game)
{
    Q_CHECK_PTR(game);
    Q_ASSERT(finalImageWidget == nullptr);

    finalImageWidget = std::make_shared<FormFinalImage>(game);

    ui->dockWidget->setWidget(finalImageWidget.get());

    connect(finalImageWidget.get(), SIGNAL(windowTitleChanged(QString)), ui->dockWidget, SLOT(setWindowTitle(QString)));
}

IGame *MainWindow::createNewGame(const SourceImage &sourceImage)
{
    DialogGameStart dialog(sourceImage, this);

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

    diskToolBox.setChildEnabled("pushButtonSaveGame", true);

    auto oldGame = game;
    game = newGame;

    if (oldGame != nullptr) {
        garbageCollector.push(oldGame);
        garbageCollector.start();
    }
}

