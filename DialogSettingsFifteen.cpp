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
#include "DialogSettingsFifteen.h"
#include "ui_DialogSettingsFifteen.h"

#include "CellSelectionGrid.h"

#include "GameCore.h"
#include "Slide/GameDataSimpleSlide.h"
#include "Swap/GameDataSimpleSwap.h"

#include "EzPuzzles.h"

#include <QDebug>

DialogSettingsFifteen::DialogSettingsFifteen(const SourceImage &sourceImage, bool showOkButton
                                           , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSettingsFifteen)
    , m_grid(QSharedPointer<CellSelectionGrid>::create(2, 2))
    , m_sourceImage(sourceImage)
{
    ui->setupUi(this);

    Q_ASSERT(!sourceImage.isNull());

    ui->buttonBox->setVisible(showOkButton);

    ui->comboBoxGameType->setItemData(0, EzPuzzles::SimpleSwap);
    ui->comboBoxGameType->setItemData(1, EzPuzzles::SimpleSlide);

    ui->imageWidget->setPixmap(sourceImage.pixmap());
    ui->imageWidget->addSubWidget(m_grid);

    ui->spinBoxSplitX->setMaximum(sourceImage.width()  / 50);
    ui->spinBoxSplitY->setMaximum(sourceImage.height() / 50);
    ui->hSliderSplitX->setMaximum(ui->spinBoxSplitX->maximum());
    ui->hSliderSplitY->setMaximum(ui->spinBoxSplitY->maximum());

    connect(ui->radioButtonBlankRandom, &QRadioButton::toggled
          , m_grid.get(), &CellSelectionGrid::setRandomSelection);

    m_grid->setRandomSelection(ui->radioButtonBlankRandom->isChecked());
}

DialogSettingsFifteen::~DialogSettingsFifteen()
{
    delete ui;
}

QSharedPointer<IGame> DialogSettingsFifteen::buildGame() const
{
    if (ui->comboBoxGameType->currentData() == EzPuzzles::SimpleSlide)
        return buildSimpleSlide();

    if (ui->comboBoxGameType->currentData() == EzPuzzles::SimpleSwap)
        return buildSimpleSwap();

    Q_ASSERT(false);

    return nullptr;
}

void DialogSettingsFifteen::udpateGrid()
{
    m_grid->setCellCount(ushort(ui->hSliderSplitX->value()), ushort(ui->hSliderSplitY->value()));
}

void DialogSettingsFifteen::on_comboBoxGameType_currentIndexChanged(int)
{
    QMap<EzPuzzles::GameType, QString> hashText = {
        {EzPuzzles::SimpleSlide, QStringLiteral("Blank position")},
        {EzPuzzles::SimpleSwap, QStringLiteral("Swap target position")},
    };

    auto gameType = EzPuzzles::GameType(ui->comboBoxGameType->currentData().toInt());

    ui->groupBoxSpecifiedPosition->setTitle(hashText[gameType]);
}

QSharedPointer<IGame> DialogSettingsFifteen::buildSimpleSlide() const
{
    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition defaultBlank;

    ui->radioButtonBlankRandom->isChecked() ? defaultBlank.randomSelect(xyCount)
                                            : defaultBlank.select(m_grid->selectedCellPos());

    auto gameData = QSharedPointer<Slide::GameDataSimpleSlide>::create(m_sourceImage, defaultBlank, xyCount);

    return QSharedPointer<GameCore>::create(gameData);
}

QSharedPointer<IGame> DialogSettingsFifteen::buildSimpleSwap() const
{
    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition swapTarget;

    ui->radioButtonBlankRandom->isChecked() ? swapTarget.randomSelect(xyCount)
                                            : swapTarget.select(m_grid->selectedCellPos());

    auto gameData = QSharedPointer<Swap::GameDataSimpleSwap>::create(m_sourceImage, swapTarget, xyCount);

    return QSharedPointer<GameCore>::create(gameData);
}
