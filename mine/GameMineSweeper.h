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
#include "IMinePiece.h"
#include "SourceImage.h"

#include <QList>
#include <QVector>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QTimer>

#include <memory>

namespace MineSweeper {

class MineLocker;

class GameMineSweeper : public IGame
{
    Q_OBJECT
public:
    static QString gameName();

    GameMineSweeper(const SourceImage &sourceImage, const QSize &xyCount, int mineCount, bool isAutoLock);
    ~GameMineSweeper() = default;

    GameID gameID() const override;

    IGame *cloneAsNewGame() const override;
    void save(const QString &saveDirPath, const QSize &screenshotSize) const override;
    bool load(const QString &loadPath) override;

    void onTickFrame() override;
    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    QString shortInformation() const override;
    SourceImage sourceImage() const override;

protected:
    typedef std::shared_ptr<IDrawer> DrawerPointer;

    void initPieces();

    MinePiecePointer &getPiece(const QPoint &pos);
    const MinePiecePointer &getPiece(const QPoint &pos) const;

    void drawAll();
    void drawChanged();
    void drawPiece(QPainter &painterBuffer, int x, int y);
    void openChaining(int x, int y);
    QString openedDescription() const;

    template<typename T> T safePieceCount() const
    {
        return static_cast<T>(xy.width() * xy.height() - mineCount);
    }

    GameID gameId;

    SourceImage sourceImg;
    QPixmap backBuffer;

    QSize xy;
    int mineCount;

    QVector<QVector<MinePiecePointer>> pieces;
    QList<QPoint> changedPositions;

    DrawerPointer drawer;
    std::unique_ptr<MineLocker> mineLocker;

    int openedCount;
    int missedCount;

    bool isInited;

    QTimer timer;

private:
    friend class MineSweeperSaveData;
    GameMineSweeper();
};

} // MineSweeper

#endif // GAMEMINESWEEPER_H
