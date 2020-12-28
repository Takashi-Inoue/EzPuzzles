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
#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QFrame>
#include <QPixmap>
#include <QList>
#include <QSharedPointer>

class ISubWidget;

class ImageWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    virtual void setPixmap(const QPixmap &pixmap);
    void addSubWidget(ISubWidget *subWidget);
    void replaceSubWidget(int index, ISubWidget *subWidget);

    const QPixmap &originalPixmap() const;
    QRect imageRect() const;
    double imageScale() const;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

    QPixmap pixmap;
    QRect imageRectangle;

private:
    void calcImageRect();

    QList<QSharedPointer<ISubWidget>> subWidgets;
};

#endif // IMAGEWIDGET_H
