/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FORMSETTINGSLIKEFIFTEEN_H
#define FORMSETTINGSLIKEFIFTEEN_H

#include <QWidget>

namespace Ui {
class FormSettingsLikeFifteen;
}

class CellSelectionGrid;
class IGame;
class ImageWidget;

class FormSettingsLikeFifteen : public QWidget
{
    Q_OBJECT

public:
    explicit FormSettingsLikeFifteen(QWidget *parent = nullptr);
    ~FormSettingsLikeFifteen() override;

    QSharedPointer<IGame> buildGame() const;
    void setImageWidget(ImageWidget *imageWidget);

protected:
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QSharedPointer<IGame> buildSimpleSlide() const;
    QSharedPointer<IGame> buildSimpleSwap() const;

    Ui::FormSettingsLikeFifteen *ui;

    ImageWidget *m_imageWidget = nullptr;
    QSharedPointer<CellSelectionGrid> m_grid;

private slots:
    void onImageChanged();
    void udpateGrid();

    void on_comboBoxGameType_currentIndexChanged(int index);
};

#endif // FORMSETTINGSLIKEFIFTEEN_H
