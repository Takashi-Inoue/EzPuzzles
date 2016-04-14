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
#include "GameWidget.h"
#include "IGame.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GameWidget::GameWidget(QWidget *parent) :
    ImageWidget(parent),
    game(nullptr)
{
}

void GameWidget::setGame(IGame *game)
{
    if (this->game != nullptr)
        this->game->disconnect();

    this->game = game;

    connect(game, SIGNAL(screenUpdated()), this, SLOT(repaint()));
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    ImageWidget::paintEvent(event);

    if (game == nullptr)
        return;

    QPainter painter(this);

    game->draw(painter);
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    ImageWidget::mousePressEvent(event);

    game->click(size(), event->pos());
}
