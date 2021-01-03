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
#include "DialogSettingsMineSweeper.h"
#include "ui_DialogSettingsMineSweeper.h"

#include "MineSweeper/GameCoreMineSweeper.h"
#include "MineSweeper/GameDataMineSweeper.h"
#include "SubFrame.h"
#include "SourceImage.h"

#include <QDebug>

DialogSettingsMineSweeper::DialogSettingsMineSweeper(const SourceImage &sourceImage
                                                   , bool showOkButton, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSettingsMineSweeper)
    , m_sourceImage(sourceImage)
{
    ui->setupUi(this);

    ui->buttonBox->setVisible(showOkButton);
    ui->imageWidget->setPixmap(sourceImage.pixmap());
    ui->labelImageSize->setText(QStringLiteral("%1 x %2 pixels")
                               .arg(sourceImage.width()).arg(sourceImage.height()));

    updateLabels(ui->sliderPieceSize->value());
    updateMineMax(ui->sliderPieceSize->value());
    updateBlockPiece(ui->sliderPieceSize->value());

    ui->frameCellSize->installEventFilter(this);
}

DialogSettingsMineSweeper::~DialogSettingsMineSweeper()
{
    delete ui;
}

QSharedPointer<IGame> DialogSettingsMineSweeper::buildGame() const
{
    QSize boardXYCount(xyCount());

    QSize fieldPixelSize(boardXYCount * ui->sliderPieceSize->value());
    QPoint tl = m_subFrame->pos();

    QPixmap pixmap = ui->imageWidget->originalPixmap().copy(QRect(tl, fieldPixelSize));

    SourceImage sourceImage(m_sourceImage.fullPath(), pixmap);
    int mineCount = ui->spinBoxMineCount->value();
    bool isAutoLock = ui->checkBoxAutoLock->isChecked();

    auto gameData = QSharedPointer<MineSweeper::GameDataMineSweeper>::create(
                        sourceImage, boardXYCount, mineCount, isAutoLock);

    return QSharedPointer<MineSweeper::GameCoreMineSweeper>::create(gameData);
}

void DialogSettingsMineSweeper::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    updateSubFrame(ui->sliderPieceSize->value());
}

void DialogSettingsMineSweeper::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    updateSubFrame(ui->sliderPieceSize->value());
}

bool DialogSettingsMineSweeper::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != ui->frameCellSize || event->type() != QEvent::Paint)
        return QDialog::eventFilter(obj, event);

    if (m_blockPiece == nullptr)
        return QDialog::eventFilter(obj, event);

    QPointF tl((ui->frameCellSize->geometry().width()  - ui->sliderPieceSize->value()) / 2.0,
               (ui->frameCellSize->geometry().height() - ui->sliderPieceSize->value()) / 2.0);

    QPainter painter(ui->frameCellSize);

    m_blockPiece->draw(painter, tl);

    return QDialog::eventFilter(obj, event);
}

void DialogSettingsMineSweeper::updateLabels(int piecePixelSide)
{
    ui->labelPixels->setText(QString("%1 x %1 pixels").arg(piecePixelSide));

    QSize xy = xyCount();
    QString text = QStringLiteral("%1 x %2 : %3 cells")
                  .arg(xy.width()).arg(xy.height()).arg(xy.width() * xy.height());

    ui->labelCells->setText(text);
}

void DialogSettingsMineSweeper::updateMineMax(int piecePixelSide)
{
    QSize pixmapSize(ui->imageWidget->originalPixmap().size());

    int cellnX = pixmapSize.width()  / piecePixelSide;
    int cellnY = pixmapSize.height() / piecePixelSide;

    ui->spinBoxMineCount->setMaximum(int(cellnX * cellnY * 0.2));
}

void DialogSettingsMineSweeper::updateSubFrame(int piecePixelSide)
{
    QSize frameSize = xyCount() * piecePixelSide;

    m_subFrame = QSharedPointer<SubFrame>::create(frameSize, ui->imageWidget->originalPixmap().size(), true);

    ui->imageWidget->replaceSubWidget(0, m_subFrame);
}

void DialogSettingsMineSweeper::updateBlockPiece(int piecePixelSide)
{
    QSize size(piecePixelSide, piecePixelSide);

    m_blockPiece.reset(new BlockPiece(size));

    ui->frameCellSize->repaint();
}

void DialogSettingsMineSweeper::on_pushButtonEasy_clicked()
{
    ui->spinBoxMineCount->setValue(numberOfAllCells() * 6 / 100);
}

void DialogSettingsMineSweeper::on_pushButtonNormal_clicked()
{
    ui->spinBoxMineCount->setValue(numberOfAllCells() * 10 / 100);
}

void DialogSettingsMineSweeper::on_pushButtonHard_clicked()
{
    ui->spinBoxMineCount->setValue(numberOfAllCells() * 15 / 100);
}

void DialogSettingsMineSweeper::on_pushButtonMax_clicked()
{
    ui->spinBoxMineCount->setValue(ui->spinBoxMineCount->maximum());
}

QSize DialogSettingsMineSweeper::xyCount() const
{
    int cellSize = ui->sliderPieceSize->value();
    QSize pixmapSize(ui->imageWidget->originalPixmap().size());

    // Don't use "pixmapSize / cellSize" to calc.
    return QSize(pixmapSize.width() / cellSize, pixmapSize.height() / cellSize);
}

int DialogSettingsMineSweeper::numberOfAllCells() const
{
    const QSize &xy = xyCount();

    return xy.width() * xy.height();
}

