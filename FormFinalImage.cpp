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

#include "IGame.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

FormFinalImage::FormFinalImage(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

void FormFinalImage::setGame(QSharedPointer<IGame> game)
{
    if (m_game == game)
        return;

    if (m_game != nullptr)
        m_game->disconnect();

    m_game = game;

    setWindowTitle(m_game->shortInformation());

    connect(m_game.get(), &IGame::informationUpdated, this, &QWidget::setWindowTitle);
    connect(m_game.get(), &IGame::finalImageUpdated, this, qOverload<>(&QWidget::repaint));
}

void FormFinalImage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_game == nullptr)
        return;

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    m_game->drawFinalImage(painter);
}
