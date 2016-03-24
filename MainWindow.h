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
#include <QPixmap>
#include <memory>

namespace Ui {
class MainWindow;
}

class FormFinalImage;
class GameWidget;
class IGame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *) override;

private slots:
    void onMenuWindowAboutToShow();
    void on_action_Open_triggered();
    void on_actionNew_Game_Current_image_triggered();
    void on_action_Final_image_triggered(bool checked);
    void on_actionE_xit_triggered();

private:
    void createFinalImageWidget();
    void startNewGame(QPixmap sourcePixmap);

    Ui::MainWindow *ui;

    QPixmap originalPixmap;
    std::shared_ptr<IGame> game;
    std::shared_ptr<QPixmap> backBuffer;
    std::shared_ptr<FormFinalImage> finalImageWidget;

    GameWidget *gameWidget;
};

#endif // MAINWINDOW_H
