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

#include "ISubWidget.h"

#include <QPainter>
#include <QMouseEvent>
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

void ImageWidget::addSubWidget(ISubWidget *subWidget)
{
    subWidgets.push_back(std::shared_ptr<ISubWidget>(subWidget));

    connect(subWidget, SIGNAL(updated()), this, SLOT(repaint()));
}

void ImageWidget::replaceSubWidget(int index, ISubWidget *subWidget)
{
    if (subWidgets.empty()) {
        addSubWidget(subWidget);
        return;
    }

    Q_ASSERT(index >= 0 && index < subWidgets.size());

    subWidgets[index]->disconnect();
    subWidgets[index].reset(subWidget);

    connect(subWidget, SIGNAL(updated()), this, SLOT(repaint()));
}

const QPixmap &ImageWidget::originalPixmap() const
{
    return pixmap;
}

QRect ImageWidget::imageRect() const
{
    Q_ASSERT(!pixmap.isNull());

    return imageRectangle;
}

double ImageWidget::imageScale() const
{
    Q_ASSERT(!pixmap.isNull());

    return static_cast<double>(imageRect().width()) / pixmap.width();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    calcImageRect();

    if (imageRectangle.size() == size()) {
        painter.drawPixmap(0, 0, pixmap);
        return;
    }

    painter.drawPixmap(imageRectangle, pixmap, pixmap.rect());

    painter.setClipping(true);
    painter.setClipRect(imageRectangle);

    for (auto &subWidget : subWidgets)
        subWidget->draw(painter);
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    calcImageRect();
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    for (auto &subWidget : subWidgets)
        subWidget->mousePress(event->pos());
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);

    for (auto &subWidget : subWidgets)
        subWidget->mouseRelease(event->pos());
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    QFrame::mouseMoveEvent(event);

    for (auto &subWidget : subWidgets)
        subWidget->mouseMove(event->pos());
}

void ImageWidget::calcImageRect()
{
    if (pixmap.isNull()) {
        imageRectangle = QRect();
        return;
    }

    QSize pixSize = pixmap.size();

    pixSize.scale(size(), Qt::KeepAspectRatio);

    imageRectangle.setTop((size().height() - pixSize.height()) / 2);
    imageRectangle.setLeft((size().width()  - pixSize.width())  / 2);

    imageRectangle.setSize(pixSize);
}
