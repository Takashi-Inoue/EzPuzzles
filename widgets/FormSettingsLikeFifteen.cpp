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

#include "FormSettingsLikeFifteen.h"
#include "ui_FormSettingsLikeFifteen.h"

#include "Application.h"

#include "GameCore.h"
#include "Slide/GameDataSimpleSlide.h"
#include "Swap/GameDataSimpleSwap.h"

#include "ImageWidget/CellSelectionGrid.h"
#include "ImageWidget/ImageWidget.h"

FormSettingsLikeFifteen::FormSettingsLikeFifteen(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormSettingsLikeFifteen)
    , m_grid(QSharedPointer<CellSelectionGrid>::create(2, 2))
{
    ui->setupUi(this);

    ui->comboBoxGameType->setItemData(0, Application::SimpleSwap);
    ui->comboBoxGameType->setItemData(1, Application::SimpleSlide);

    connect(ui->radioButtonBlankRandom, &QRadioButton::toggled
          , m_grid.get(), &CellSelectionGrid::setRandomSelection);

    m_grid->setRandomSelection(ui->radioButtonBlankRandom->isChecked());
}

FormSettingsLikeFifteen::~FormSettingsLikeFifteen()
{
    delete ui;
}

QSharedPointer<IGame> FormSettingsLikeFifteen::buildGame() const
{
    if (ui->comboBoxGameType->currentData() == Application::SimpleSlide)
        return buildSimpleSlide();

    if (ui->comboBoxGameType->currentData() == Application::SimpleSwap)
        return buildSimpleSwap();

    Q_ASSERT(false);

    return nullptr;
}

void FormSettingsLikeFifteen::setImageWidget(ImageWidget *imageWidget)
{
    Q_CHECK_PTR(imageWidget);

    m_imageWidget = imageWidget;
    m_imageWidget->addSubWidget(m_grid);

    connect(m_imageWidget, &ImageWidget::imageChanged, this, &FormSettingsLikeFifteen::onImageChanged);
}

void FormSettingsLikeFifteen::hideEvent(QHideEvent *)
{
    m_grid->setVisible(false);

    if (m_imageWidget)
        m_imageWidget->setMouseTracking(false);
}

void FormSettingsLikeFifteen::showEvent(QShowEvent *)
{
    m_grid->setVisible(true);

    if (m_imageWidget)
        m_imageWidget->setMouseTracking(true);
}

QSharedPointer<IGame> FormSettingsLikeFifteen::buildSimpleSlide() const
{
    if (m_imageWidget == nullptr || !m_imageWidget->hasImage())
        return nullptr;

    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition defaultBlank;

    ui->radioButtonBlankRandom->isChecked() ? defaultBlank.randomSelect(xyCount)
                                            : defaultBlank.select(m_grid->selectedCellPos());

    SourceImage sourceImage(m_imageWidget->imageFullPathName(), m_imageWidget->originalPixmap());

    auto gameData = QSharedPointer<Slide::GameDataSimpleSlide>::create(
                        sourceImage, defaultBlank, xyCount);

    return QSharedPointer<GameCore>::create(gameData);
}

QSharedPointer<IGame> FormSettingsLikeFifteen::buildSimpleSwap() const
{
    if (m_imageWidget == nullptr || !m_imageWidget->hasImage())
        return nullptr;

    QSize xyCount(ui->hSliderSplitX->value(), ui->hSliderSplitY->value());

    UniquePosition swapTarget;

    ui->radioButtonBlankRandom->isChecked() ? swapTarget.randomSelect(xyCount)
                                            : swapTarget.select(m_grid->selectedCellPos());

    SourceImage sourceImage(m_imageWidget->imageFullPathName(), m_imageWidget->originalPixmap());

    auto gameData = QSharedPointer<Swap::GameDataSimpleSwap>::create(
                        sourceImage, swapTarget, xyCount);

    return QSharedPointer<GameCore>::create(gameData);
}

void FormSettingsLikeFifteen::onImageChanged()
{
    setEnabled(m_imageWidget->hasImage());

    if (!isEnabled())
        return;

    ui->spinBoxSplitX->setMaximum(m_imageWidget->originalPixmap().width()  / 50);
    ui->spinBoxSplitY->setMaximum(m_imageWidget->originalPixmap().height() / 50);
    ui->hSliderSplitX->setMaximum(ui->spinBoxSplitX->maximum());
    ui->hSliderSplitY->setMaximum(ui->spinBoxSplitY->maximum());
}

void FormSettingsLikeFifteen::udpateGrid()
{
    m_grid->setCellCount(ushort(ui->hSliderSplitX->value()), ushort(ui->hSliderSplitY->value()));
}

void FormSettingsLikeFifteen::on_comboBoxGameType_currentIndexChanged(int /*index*/)
{
    QMap<Application::GameType, QString> hashText = {
        {Application::SimpleSlide, QStringLiteral("Blank position")},
        {Application::SimpleSwap, QStringLiteral("Swap target position")},
    };

    auto gameType = Application::GameType(ui->comboBoxGameType->currentData().toInt());

    ui->groupBoxSpecifiedPosition->setTitle(hashText[gameType]);
}
