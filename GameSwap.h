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
#ifndef GAMESWAP_H
#define GAMESWAP_H

#include "PiecesGame.h"

#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QSize>

class GameSwap : public PiecesGame
{
    Q_OBJECT
public:
    GameSwap(const QPixmap &sourcePixmap, const QSize &xyCount, QObject *parent = 0);
    ~GameSwap() = default;

    QString shortInformation() const override;

protected:
    void clickOperation(const QSize &fieldSize, const QPoint &cursorPos) override;

private:
};

#endif // GAMESWAP_H
