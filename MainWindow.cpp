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

#include "DialogSavedata.h"
#include "FormFinalImage.h"

#include "IGame.h"
#include "GameWidget.h"

#include "Application.h"
#include "ImageHistory.h"
#include "SourceImage.h"

#include "widgets/DialogStartGame.h"

#include <QSettings>
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

    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    QVariant savedGeometry = settings.value(m_geometryKey);
    QVariant savedState = settings.value(m_windowStateKey);

    if (savedGeometry.isValid())
        restoreGeometry(savedGeometry.toByteArray());

    if (savedState.isValid())
        restoreState(savedState.toByteArray());

    ui->gameWidget->resize(1, 1);

    connect(ui->widgetFinalImage, &QWidget::windowTitleChanged, ui->dockWidget, &QWidget::setWindowTitle);

    connect(m_threadFrameTimer, &ThreadFrameTimer::tick, this, &MainWindow::onTickFrameTimer);

    m_threadFrameTimer->start();

    gameThread.start();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings settings(Application::iniFilePathName(), QSettings::IniFormat);

    settings.setValue(m_geometryKey, saveGeometry());
    settings.setValue(m_windowStateKey, saveState());

    m_threadFrameTimer->stop();
    m_threadFrameTimer->wait();

    gameThread.quit();
    gameThread.wait();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionFinalImage_triggered(bool checked)
{
    m_isFinalImageShownFromUser = checked;
}

void MainWindow::on_actionNewGame_triggered()
{
    QString currentImage = (m_game != nullptr) ? m_game->sourceImage().fullPath()
                                               : QString();

    DialogStartGame dialog(currentImage, this);

    if (dialog.exec() == QDialog::Rejected)
        return;

    QSharedPointer<IGame> game = dialog.buildGame();

    if (game == nullptr)
        return;

    startNewGame(game);
    updateImageHistory(game->sourceImage().fullPath());
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

void MainWindow::saveGame()
{
    if (m_game == nullptr)
        return;

    Application::createUserDataDir();
    m_game->save(Application::userDataDirPath(), Application::screenshotSize());
}

void MainWindow::loadGame()
{
    DialogSavedata dialog(this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QSharedPointer<IGame> game = dialog.loadGame();

    if (game == nullptr)
        return;

    updateTitle(game->shortInformation());
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
    ImageHistory history;

    history.load();
    history.addString(lastImagePath);
    history.save();
}

void MainWindow::startNewGame(QSharedPointer<IGame> newGame)
{
    Q_CHECK_PTR(newGame);

    if (newGame == m_game)
        return;

    if (m_game != nullptr)
        m_game->disconnect();

    bool hasFinalImage = newGame->hasFinalImage();

    ui->actionFinalImage->setChecked(m_isFinalImageShownFromUser & hasFinalImage);
    ui->actionFinalImage->setEnabled(hasFinalImage);
    ui->widgetFinalImage->setEnabled(hasFinalImage);

    if (hasFinalImage) {
        ui->widgetFinalImage->setGame(newGame);
        ui->widgetFinalImage->repaint();
    }

    ui->gameWidget->resize(newGame->maxFieldSize());
    ui->gameWidget->setGame(newGame);

    ui->actionRestart->setEnabled(true);
    ui->actionSave->setEnabled(true);

    updateTitle(newGame->shortInformation());

    connect(newGame.get(), &IGame::informationUpdated, this, &MainWindow::updateTitle);

    m_game = newGame;
}
