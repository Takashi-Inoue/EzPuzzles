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
#ifndef DIALOGSETTINGSFIFTEEN_H
#define DIALOGSETTINGSFIFTEEN_H

#include "IDialogGameSettings.h"
#include <QDialog>

class IGame;
class SourceImage;

namespace Ui {
class DialogSettingsFifteen;
}

class DialogSettingsFifteen : public QDialog, public IDialogGameSettings
{
    Q_OBJECT

public:
    explicit DialogSettingsFifteen(const SourceImage &sourceImage, bool showOkButton = true, QWidget *parent = 0);
    ~DialogSettingsFifteen();

    IGame *buildGame() const override;

private slots:
    void updateSplitPainter() const;

private:
    const int idFifteen = 0;
    const int idSwap = 1;

    IGame *buildSimpleSlide() const;
    IGame *buildSimpleSwap() const;

    Ui::DialogSettingsFifteen *ui;

    const SourceImage &sourceImage;
};

#endif // DIALOGSETTINGSFIFTEEN_H
