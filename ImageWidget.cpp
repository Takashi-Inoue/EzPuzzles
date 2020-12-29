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

#include "ISubWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

void ImageWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

void ImageWidget::addSubWidget(QSharedPointer<ISubWidget> subWidget)
{
    m_subWidgets.push_back(subWidget);

    connect(subWidget.get(), &ISubWidget::updated, this, qOverload<>(&QWidget::repaint));
}

void ImageWidget::replaceSubWidget(int index, QSharedPointer<ISubWidget> subWidget)
{
    if (m_subWidgets.empty()) {
        addSubWidget(subWidget);
        return;
    }

    Q_ASSERT(index >= 0 && index < m_subWidgets.size());

    m_subWidgets[index]->disconnect();
    m_subWidgets[index] = subWidget;

    connect(subWidget.get(), &ISubWidget::updated, this, qOverload<>(&QWidget::repaint));
}

const QPixmap &ImageWidget::originalPixmap() const
{
    return m_pixmap;
}

QRect ImageWidget::imageRect() const
{
    Q_ASSERT(!m_pixmap.isNull());

    return m_imageRectangle;
}

double ImageWidget::imageScale() const
{
    Q_ASSERT(!m_pixmap.isNull());

    return double(imageRect().width()) / m_pixmap.width();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    calcImageRect();

    if (m_imageRectangle.size() == size()) {
        painter.drawPixmap(0, 0, m_pixmap);
        return;
    }

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

    pixSize.scale(size(), Qt::KeepAspectRatio);

    m_imageRectangle.setTop((size().height() - pixSize.height()) / 2);
    m_imageRectangle.setLeft((size().width()  - pixSize.width())  / 2);

    m_imageRectangle.setSize(pixSize);
}
