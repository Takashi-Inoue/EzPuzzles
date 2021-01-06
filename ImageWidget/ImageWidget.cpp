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

#include "AbstractSubWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

int ImageWidget::addSubWidget(QSharedPointer<AbstractSubWidget> subWidget)
{
    m_subWidgets << subWidget;

    connect(subWidget.get(), &AbstractSubWidget::updated, this, qOverload<>(&QWidget::repaint));

    return int(m_subWidgets.size() - 1);
}

int ImageWidget::replaceSubWidget(int index, QSharedPointer<AbstractSubWidget> subWidget)
{
    if (subWidget == nullptr)
        return -1;

    if (index == -1 || index >= m_subWidgets.size())
        return addSubWidget(subWidget);

    m_subWidgets[index]->disconnect();
    m_subWidgets[index] = subWidget;

    connect(subWidget.get(), &AbstractSubWidget::updated, this, qOverload<>(&QWidget::repaint));

    return index;
}

QSharedPointer<AbstractSubWidget> ImageWidget::subWidget(int index) const
{
    if (uint(index) < uint(m_subWidgets.size()))
        return m_subWidgets[index];

    return nullptr;
}

bool ImageWidget::hasImage() const
{
    return !m_pixmap.isNull();
}

const QPixmap &ImageWidget::originalPixmap() const
{
    return m_pixmap;
}

QString ImageWidget::imageFullPathName() const
{
    return m_imageFullPathName;
}

QRect ImageWidget::imageRect() const
{
    return m_imageRectangle;
}

double ImageWidget::imageScale() const
{
    Q_ASSERT(!m_pixmap.isNull());

    return double(m_imageRectangle.width()) / m_pixmap.width();
}

void ImageWidget::setImage(QStringView imageFullPathName)
{
    QImage image(imageFullPathName.toString());

    if (image.isNull())
        return;

    m_imageFullPathName = imageFullPathName.toString();

    m_pixmap = QPixmap::fromImage(image);

    repaint();

    emit imageChanged();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (m_pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    calcImageRect();

    painter.drawPixmap(m_imageRectangle, m_pixmap, m_pixmap.rect());

    painter.setClipping(true);
    painter.setClipRect(m_imageRectangle);

    for (auto &subWidget : m_subWidgets)
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

    for (auto &subWidget : m_subWidgets)
        subWidget->mousePress(event);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);

    for (auto &subWidget : m_subWidgets)
        subWidget->mouseRelease(event);
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    QFrame::mouseMoveEvent(event);

    for (auto &subWidget : m_subWidgets)
        subWidget->mouseMove(event);
}

void ImageWidget::enterEvent(QEnterEvent *event)
{
    QFrame::enterEvent(event);

    for (auto &subWidget : m_subWidgets)
        subWidget->mouseEnter(event);
}

void ImageWidget::leaveEvent(QEvent *event)
{
    QFrame::leaveEvent(event);

    for (auto &subWidget : m_subWidgets)
        subWidget->mouseLeave(event);
}

void ImageWidget::calcImageRect()
{
    if (m_pixmap.isNull()) {
        m_imageRectangle = QRect();
        return;
    }

    QSize pixSize = m_pixmap.size();
    int frame2Width = frameWidth() * 2;

    pixSize.scale(size() - QSize(frame2Width, frame2Width), Qt::KeepAspectRatio);

    m_imageRectangle.setSize(pixSize);
    m_imageRectangle.moveCenter(rect().center());
}
