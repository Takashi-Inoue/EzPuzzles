﻿/*
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
#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "MineSweeper/MinePiece.h"
#include "MineSweeper/MineLocker.h"

#include <QList>
#include <memory>

namespace MineSweeper {

class SaveDataMineSweeper;

class MineField
{
public:
    MineField(QVector<QVector<MinePiecePointer>> &pieces, MineLockerPointer mineLocker, int mineCount);

    void open(const QPoint &pos);
    void save(SaveDataMineSweeper &) const;
    void load(const SaveDataMineSweeper &);

    double openedRate() const;
    double mineRatio() const;
    bool isAllOpened() const;
    bool isNoMissed() const;

    QString information() const;
    const QList<QPoint> &explodedPositions() const;

private:
    void openChaining(const QPoint &pos);

    int totalPieceCount() const;
    int safePiecesCount() const;

    QVector<QVector<MinePiecePointer>> &pieces;
    int mineCount;
    int openedCount;
    int missedCount;

    MineLockerPointer mineLocker;

    QList<QPoint> explodedPos;
};

typedef std::shared_ptr<MineField> MineFieldPointer;

} // MineSweeper

#endif // MINEFIELD_H