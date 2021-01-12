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

#include "FormSettingsMineSweeper.h"
#include "ui_FormSettingsMineSweeper.h"

#include "BlockPiece.h"
#include "ImageWidget/ImageWidget.h"
#include "ImageWidget/SubFrame.h"
#include "MineSweeper/GameCoreMineSweeper.h"
#include "MineSweeper/GameDataMineSweeper.h"

#include <QDebug>

FormSettingsMineSweeper::FormSettingsMineSweeper(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormSettingsMineSweeper)
{
    ui->setupUi(this);

    ui->pushButtonEasy  ->setProperty(m_propertyMineRatio,  6);
    ui->pushButtonNormal->setProperty(m_propertyMineRatio, 10);
    ui->pushButtonHard  ->setProperty(m_propertyMineRatio, 15);
    ui->pushButtonMax   ->setProperty(m_propertyMineRatio, 20);

    updateLabels();
    updateMineMax();
    updateSubFrame();

    ui->frameCellSize->installEventFilter(this);

    connect(ui->sliderPieceSize, &QSlider::valueChanged
          , ui->frameCellSize, qOverload<>(&QWidget::repaint));
}

QSharedPointer<IGame> FormSettingsMineSweeper::buildGame() const
{
    if (m_subFrameIndex == -1)
        return nullptr;

    QSize boardXYCount(xyCount());

    QSize fieldPixelSize(boardXYCount * ui->sliderPieceSize->value());
    QPoint tl = m_imageWidget->subWidget(m_subFrameIndex)->pos();

    QPixmap pixmap = m_imageWidget->originalPixmap().copy(QRect(tl, fieldPixelSize));

    SourceImage sourceImage(m_imageWidget->imageFullPathName(), pixmap);
    int mineCount = qMax(1, ui->spinBoxMineCount->value());
    bool isAutoLock = ui->checkBoxAutoLock->isChecked();

    auto gameData = QSharedPointer<MineSweeper::GameDataMineSweeper>::create(
                        sourceImage, boardXYCount, mineCount, isAutoLock);

    return QSharedPointer<MineSweeper::GameCoreMineSweeper>::create(gameData);
}

FormSettingsMineSweeper::~FormSettingsMineSweeper()
{
    delete ui;
}

void FormSettingsMineSweeper::setImageWidget(ImageWidget *imageWidget)
{
    Q_CHECK_PTR(imageWidget);

    m_imageWidget = imageWidget;

    connect(m_imageWidget, &ImageWidget::imageChanged, this, [this]() {
        setEnabled(m_imageWidget->hasImage());

        updateLabels();
        updateMineMax();
        updateSubFrame();
    });
}

bool FormSettingsMineSweeper::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != ui->frameCellSize || event->type() != QEvent::Paint)
        return QWidget::eventFilter(obj, event);

    const int pieceSide = ui->sliderPieceSize->value();
    QPixmap pixmap(pieceSide, pieceSide);
    QPainter painterPixmap(&pixmap);

    BlockPiece(pixmap.size()).draw(painterPixmap, QPointF(0, 0));

    QPointF tl(ui->frameCellSize->width()  - pieceSide
             , ui->frameCellSize->height() - pieceSide);
    QIcon icon(pixmap);
    QIcon::Mode iconMode = isEnabled() ? QIcon::Normal : QIcon::Disabled;

    QPainter(ui->frameCellSize).drawPixmap(tl /= 2.0, icon.pixmap(pieceSide, iconMode));

    return QWidget::eventFilter(obj, event);
}

void FormSettingsMineSweeper::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    updateSubFrame();
}

void FormSettingsMineSweeper::hideEvent(QHideEvent *)
{
    if (m_subFrameIndex == -1)
        return;

    m_imageWidget->subWidget(m_subFrameIndex)->setVisible(false);
}

void FormSettingsMineSweeper::showEvent(QShowEvent *)
{
    if (m_subFrameIndex == -1)
        return;

    m_imageWidget->subWidget(m_subFrameIndex)->setVisible(true);
}

void FormSettingsMineSweeper::onRatioButtonClicked(QAbstractButton *button)
{
    int ratio = button->property(m_propertyMineRatio).toInt();

    ui->spinBoxMineCount->setValue((cellCount() * ratio) / 100);
}

QSize FormSettingsMineSweeper::xyCount() const
{
    if (m_imageWidget == nullptr || !m_imageWidget->hasImage())
        return QSize(0, 0);

    int cellSize = ui->sliderPieceSize->value();
    QSize pixmapSize(m_imageWidget->originalPixmap().size());

    // Don't use "pixmapSize / cellSize" to calc.
    return QSize(pixmapSize.width() / cellSize, pixmapSize.height() / cellSize);
}

int FormSettingsMineSweeper::cellCount() const
{
    const QSize &xy = xyCount();

    return xy.width() * xy.height();
}

void FormSettingsMineSweeper::updateLabels()
{
    ui->labelPixels->setText(QString("%1 x %1 pixels").arg(ui->sliderPieceSize->value()));

    QSize xy = xyCount();
    QString text = QStringLiteral("%1 x %2 : %3 cells")
                  .arg(xy.width()).arg(xy.height()).arg(xy.width() * xy.height());

    ui->labelCells->setText(text);
}

void FormSettingsMineSweeper::updateMineMax()
{
    int ratio = ui->pushButtonMax->property(m_propertyMineRatio).toInt();

    ui->spinBoxMineCount->setMaximum((cellCount() * ratio) / 100);
}

void FormSettingsMineSweeper::updateSubFrame()
{
    if (m_imageWidget == nullptr || !m_imageWidget->hasImage())
        return;

    const QPixmap &pixmap = m_imageWidget->originalPixmap();

    QSize frameSize = xyCount() * ui->sliderPieceSize->value();
    QPoint pos = (m_subFrameIndex != -1) ? m_imageWidget->subWidget(m_subFrameIndex)->pos()
                                         : QPoint(0, 0);

    auto subFrame = QSharedPointer<SubFrame>::create(frameSize, pixmap.size(), true, pos);

    subFrame->setVisible(isVisible());

    m_subFrameIndex = m_imageWidget->replaceSubWidget(m_subFrameIndex, subFrame);
    m_imageWidget->repaint();
}
