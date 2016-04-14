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
#include "FormFinalImage.h"
#include "ui_FormFinalImage.h"

#include "IGame.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

FormFinalImage::FormFinalImage(IGame *game, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFinalImage),
    game(game),
    grabbedEdges(0)
{
    Q_CHECK_PTR(game);

    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

FormFinalImage::~FormFinalImage()
{
    delete ui;
}

void FormFinalImage::setGame(IGame *game)
{
    this->game = game;
}

void FormFinalImage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    game->drawFinalImage(painter);
    setWindowTitle(game->shortInformation());
}

void FormFinalImage::resizeEvent(QResizeEvent *event)
{
    if (newSize.isValid()) {
        QWidget::resizeEvent(event);
        return;
    }

    newSize = game->maxFieldSize();

    int w = width();
    int h = height();
    int subW = w - event->oldSize().width();
    int subH = h - event->oldSize().height();

    qAbs(subW) > qAbs(subH) ? newSize.scale(w, INT_MAX, Qt::KeepAspectRatio)
                            : newSize.scale(INT_MAX, h, Qt::KeepAspectRatio);

    resize(newSize);

    if ((grabbedEdges & Qt::LeftEdge) && (grabbedEdges & Qt::TopEdge)) {
        grabbedEdges = 0;
        move(QCursor::pos());
    } else if (grabbedEdges & Qt::LeftEdge) {
        grabbedEdges = 0;
        move(QCursor::pos().x(), y());
    } else if (grabbedEdges & Qt::TopEdge) {
        grabbedEdges = 0;
        move(x(), QCursor::pos().y());
    }
}

bool FormFinalImage::event(QEvent *e)
{
    if (e->type() == QEvent::NonClientAreaMouseButtonPress) {
        grabbedEdges = 0;
        newSize = QSize();

        QPoint mpos = QCursor::pos();

        if (mpos.x() >= x() && mpos.x() < geometry().x())
            grabbedEdges |= Qt::LeftEdge;
        else if (mpos.x() > geometry().right() && mpos.x() <= frameGeometry().right())
            grabbedEdges |= Qt::RightEdge;

        if (mpos.y() >= y() && mpos.y() < geometry().y())
            grabbedEdges |= Qt::TopEdge;
        else if (mpos.y() > geometry().bottom() && mpos.y() <= frameGeometry().bottom())
            grabbedEdges |= Qt::BottomEdge;
    }

    return QWidget::event(e);
}
