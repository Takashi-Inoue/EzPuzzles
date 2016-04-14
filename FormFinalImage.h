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
#ifndef FORMFINALIMAGE_H
#define FORMFINALIMAGE_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class FormFinalImage;
}

class IGame;

class FormFinalImage : public QWidget
{
    Q_OBJECT

public:
    explicit FormFinalImage(IGame *game, QWidget *parent = 0);
    ~FormFinalImage();

    void setGame(IGame *game);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *) override;

private:
    Ui::FormFinalImage *ui;
    IGame *game;
    int grabbedEdges;
    QSize newSize;
};

#endif // FORMFINALIMAGE_H
