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

#include "FrameImageSelector.h"
#include "ui_FrameImageSelector.h"

FrameImageSelector::FrameImageSelector(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameImageSelector)
{
    ui->setupUi(this);

    connect(ui->imageWidget, SIGNAL(imageChanged()), this, SLOT(onImageChanged()));
    connect(ui->imageWidget, SIGNAL(imageChanged()), this, SIGNAL(imageChanged()));

    for (auto actionSelectImage : findChildren<AbstractSelectImageToolButton *>()) {
        connect(actionSelectImage, &AbstractSelectImageToolButton::imageSelected
              , ui->imageWidget, &ImageWidget::setImage);
    }
}

FrameImageSelector::~FrameImageSelector()
{
    delete ui;
}

bool FrameImageSelector::hasImage() const
{
    return !ui->imageWidget->originalPixmap().isNull();
}

const QPixmap &FrameImageSelector::pixmap() const
{
    return ui->imageWidget->originalPixmap();
}

QString FrameImageSelector::imageFullPathName() const
{
    return ui->imageWidget->imageFullPathName();
}

void FrameImageSelector::onImageChanged()
{
    QSize imageSize = ui->imageWidget->originalPixmap().size();

    ui->labelImageSize->setText(QStringLiteral("%1 x %2 pixels")
                               .arg(imageSize.width()).arg(imageSize.height()));
}
