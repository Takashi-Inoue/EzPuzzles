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
#include <QThread>
#include <QDebug>

#include <QMenu>

QThread gameThread;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_threadFrameTimer(new ThreadFrameTimer(120, this))
{
    qDebug() << "MainThread :" << thread()->currentThreadId();

    ui->setupUi(this);

    ui->gameWidget->resize(1, 1);

    connect(ui->widgetFinalImage, &QWidget::windowTitleChanged, ui->dockWidget, &QWidget::setWindowTitle);
    connect(ui->widgetFinalImage, &QWidget::windowTitleChanged, this, &MainWindow::updateTitle);

    connect(m_threadFrameTimer, &ThreadFrameTimer::tick, this, &MainWindow::onTickFrameTimer);

    m_threadFrameTimer->start();

    gameThread.start();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    m_threadFrameTimer->stop();
    m_threadFrameTimer->wait();

    gameThread.quit();
    gameThread.wait();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewGame_triggered()
{
    static QMenu *menu;

    if (menu == nullptr) {
        menu = new QMenu(this);
        menu->addActions({ui->actionOpenImage, ui->actionFromHistory});
    }

    menu->popup(QCursor::pos());
}

void MainWindow::on_actionFromCurrentImage_triggered()
{
    auto newGame = createNewGame(m_game->sourceImage());

    if (newGame != nullptr)
        startNewGame(newGame);
}

void MainWindow::on_actionRestart_triggered()
{
    startNewGame(m_game->cloneAsNewGame());
}

void MainWindow::on_actionSaveLoad_triggered()
{
    static QMenu *menu;

    if (menu == nullptr) {
        menu = new QMenu(this);
        menu->addActions({ui->actionLoad, ui->actionSave});
    }

    menu->popup(QCursor::pos());
}

void MainWindow::startGameWithNewImage()
{
    QString imagePath = QFileDialog::getOpenFileName(
                            this
                          , QStringLiteral("Open image..."), QStringLiteral("")
                          , QStringLiteral("Images (*.png *.bmp *.jpg *.jpeg)"));

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
    if (m_game == nullptr)
        return;

    EzPuzzles::createSaveDirPath();
    m_game->save(EzPuzzles::saveDirPath(), EzPuzzles::screenshotSize());
}

void MainWindow::loadGame()
{
    DialogSavedata dialog(this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QSharedPointer<IGame> game = dialog.loadGame();

    if (game == nullptr)
        return;

    startNewGame(game);
    updateImageHistory(game->sourceImage().fullPath());
}

void MainWindow::onTickFrameTimer(QReadWriteLock *lock, QWaitCondition *wait)
{
    lock->lockForWrite();

//    qDebug() << "onTickFrameTimer lock";

    if (m_game != nullptr)
        m_game->onTickFrame();

    ui->gameWidget->repaint();

//    qDebug() << "onTickFrameTimer wake";
    wait->wakeAll();
    lock->unlock();
}

void MainWindow::updateTitle(QString finalImageWidgetTitle)
{
    setWindowTitle(QStringLiteral("EzPuzzles : %1").arg(finalImageWidgetTitle));
}

void MainWindow::updateImageHistory(const QString &lastImagePath) const
{
    StringListHistory history;

    history.load(EzPuzzles::imageHistoryPath());
    history.addString(lastImagePath);
    history.save(EzPuzzles::imageHistoryPath());
}

QSharedPointer<IGame> MainWindow::createNewGame(const SourceImage &sourceImage)
{
    DialogGameStart dialog(sourceImage, this);

    if (dialog.exec() == QDialog::Rejected)
        return nullptr;

    return dialog.buildGame();
}

void MainWindow::startNewGame(QSharedPointer<IGame> newGame)
{
    Q_CHECK_PTR(newGame);

    if (newGame == m_game)
        return;

    if (m_game != nullptr)
        m_game->disconnect();

    ui->widgetFinalImage->setGame(newGame);
    ui->widgetFinalImage->repaint();

    connect(newGame.get(), &IGame::informationUpdated, ui->widgetFinalImage, qOverload<>(&QWidget::repaint));

    ui->gameWidget->resize(newGame->maxFieldSize());
    ui->gameWidget->setGame(newGame);

    ui->actionFromCurrentImage->setEnabled(true);
    ui->actionRestart->setEnabled(true);
    ui->actionSave->setEnabled(true);

    m_game = newGame;
}

