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
#include "ImageWidget.h"
#include "ui_ImageWidget.h"

#include "IPainter.h"

#include <QPainter>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::setPixmap(const QPixmap &pixmap)
{
    this->pixmap = pixmap;
}

void ImageWidget::appendExtraPainter(IPainter *painter)
{
    painters.push_back(std::shared_ptr<IPainter>(painter));
}

void ImageWidget::replaceExtraPainter(int index, IPainter *painter)
{
    Q_ASSERT(index >= 0 && index < painters.size());

    painters[index].reset(painter);
}

const QPixmap &ImageWidget::originalPixmap() const
{
    return pixmap;
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QSize pixSize = pixmap.size();

    if (pixSize == size()) {
        painter.drawPixmap(0, 0, pixmap);
        return;
    }

    pixSize.scale(size(), Qt::KeepAspectRatio);

    imagePos.ry() = (size().height() - pixSize.height()) / 2;
    imagePos.rx() = (size().width()  - pixSize.width())  / 2;

    painter.drawPixmap(QRect(imagePos, pixSize), pixmap, pixmap.rect());
    painter.setClipping(true);
    painter.setClipRect(QRect(imagePos, pixSize));

    for (std::shared_ptr<IPainter> &ipainter : painters)
        ipainter->exec(painter);
}
