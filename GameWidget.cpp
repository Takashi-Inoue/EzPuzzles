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

void GameWidget::setGame(QSharedPointer<IGame> game)
{
    if (m_game == game)
        return;

    if (m_game != nullptr)
        m_game->disconnect();

    m_game = game;
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    ImageWidget::paintEvent(event);

    if (m_game == nullptr)
        return;

    QPainter painter(this);

    m_game->draw(painter);
}

//void GameWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    ImageWidget::mouseMoveEvent(event);

//    qDebug() << "mouseMove";
//}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    ImageWidget::mousePressEvent(event);

    m_game->click(size(), event->pos());
}
