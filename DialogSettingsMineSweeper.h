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
#ifndef DIALOGSETTINGSMINESWEEPER_H
#define DIALOGSETTINGSMINESWEEPER_H

#include "IDialogGameSettings.h"
#include "BlockPiece.h"

#include <QDialog>
#include <QScopedPointer>
#include <QSize>

class ISubWidget;
class SourceImage;

namespace Ui {
class DialogSettingsMineSweeper;
}

class DialogSettingsMineSweeper : public QDialog, public IDialogGameSettings
{
    Q_OBJECT

public:
    explicit DialogSettingsMineSweeper(const SourceImage &sourceImage, bool showOkButton = true
                                     , QWidget *parent = nullptr);
    ~DialogSettingsMineSweeper() override;

    // IDialogGameSettings
    QSharedPointer<IGame> buildGame() const override;

protected:
    void showEvent(QShowEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;

private slots:
    void updateLabels();
    void updateMineMax();
    void updateSubFrame();
    void updateBlockPiece();

    void on_pushButtonEasy_clicked();
    void on_pushButtonNormal_clicked();
    void on_pushButtonHard_clicked();
    void on_pushButtonMax_clicked();

private:
    QSize xyCount() const;
    int numberOfAllCells() const;

    Ui::DialogSettingsMineSweeper *ui;

    QSharedPointer<ISubWidget> m_subFrame;
    const SourceImage &m_sourceImage;

    QScopedPointer<IPiece> m_blockPiece;
};

#endif // DIALOGSETTINGSMINESWEEPER_H
