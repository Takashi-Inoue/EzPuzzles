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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "ThreadFrameTimer.h"

namespace Ui {
class MainWindow;
}

class IGame;
class SourceImage;
class ThreadFrameTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNewGame_triggered();
    void on_actionFromCurrentImage_triggered();
    void on_actionRestart_triggered();
    void on_actionSaveLoad_triggered();

    void startGameWithNewImage();
    void startGameFromImageHistory();
    void saveGame();
    void loadGame();
    void onTickFrameTimer(QReadWriteLock *lock, QWaitCondition *wait);
    void updateTitle(QString);

private:
    void updateImageHistory(const QString &lastImagePath) const;
    QSharedPointer<IGame> createNewGame(const SourceImage &sourceImage);
    void startNewGame(QSharedPointer<IGame> newGame);

    Ui::MainWindow *ui;

    ThreadFrameTimer *m_threadFrameTimer;
    QSharedPointer<IGame> m_game;
};

#endif // MAINWINDOW_H
