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
#ifndef GAMEMINESWEEPER_H
#define GAMEMINESWEEPER_H

#include "IGame.h"
#include "IDrawer.h"

#include <QList>
#include <QVector>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QTimer>

#include <memory>
#include <random>

namespace MineSweeper {

class IMinePiece;

class GameMineSweeper : public IGame
{
    Q_OBJECT
public:
    GameMineSweeper(const QPixmap &sourcePixmap, const QSize &fieldSize, const QSize &xyCount, int mineCount, bool isAutoLock, QObject *parent = 0);
    ~GameMineSweeper() = default;

    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    QString shortInformation() const override;
    QPixmap pixmap() const override;

protected:
    typedef std::shared_ptr<IMinePiece> MinePiecePointer;
    typedef std::shared_ptr<IDrawer> DrawerPointer;

    void initPieces();
    void createWallPieces();
    void createMinePieces();
    void createMinePieces(int minX, int maxX, int minY, int maxY, int numberOfMines);
    void createSafePieces();

    QList<MinePiecePointer> getAroundPieces(int x, int y) const;
    QList<QPoint> getAroundPositions(const QPoint &pos) const;
    MinePiecePointer &getPiece(const QPoint &pos);

    void drawAll();
    void drawChanged();
    void drawPiece(QPainter &painterBuffer, int x, int y);
    void openChaining(int x, int y);
    void checkMinesForLock();

    template<typename T> T safePieceCount() const
    {
        return static_cast<T>(xy.width() * xy.height() - mineCount);
    }

    QPixmap sourcePixmap;
    QPixmap backBuffer;

    const QSize xy;
    const int mineCount;
    const bool isAutoLock;

    QVector<QVector<MinePiecePointer>> pieces;
    QList<QPoint> changedPositions;
    QList<QPoint> notLockedMinesPos;

    DrawerPointer drawer;

    int opendCount;
    int missedCount;

    bool isStarted;

    std::mt19937 mt;
    QTimer timer;
};

} // MineSweeper

#endif // GAMEMINESWEEPER_H
