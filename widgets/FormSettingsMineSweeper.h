/*
 * Copyright 2021 Takashi Inoue
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

#ifndef FORMSETTINGSMINESWEEPER_H
#define FORMSETTINGSMINESWEEPER_H

#include <QWidget>

namespace Ui {
class FormSettingsMineSweeper;
}

class IGame;
class ImageWidget;

class QAbstractButton;

class FormSettingsMineSweeper : public QWidget
{
    Q_OBJECT

public:
    explicit FormSettingsMineSweeper(QWidget *parent = nullptr);
    ~FormSettingsMineSweeper() override;

    QSharedPointer<IGame> buildGame() const;
    void setImageWidget(ImageWidget *imageWidget);

protected:
    bool eventFilter(QObject *, QEvent *) override;
    void resizeEvent(QResizeEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onRatioButtonClicked(QAbstractButton *button);

private:
    static constexpr char m_propertyMineRatio[] = "mineRatio";

    QSize xyCount() const;
    int cellCount() const;

    Ui::FormSettingsMineSweeper *ui;

    ImageWidget *m_imageWidget = nullptr;

    int m_subFrameIndex = -1;

private slots:
    void updateLabels();
    void updateMineMax();
    void updateSubFrame();
};

#endif // FORMSETTINGSMINESWEEPER_H
