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
#ifndef GAMELIKEFIFTEEN_H
#define GAMELIKEFIFTEEN_H

#include "IGame.h"
#include "IPuzzlePiece.h"
#include "GameID.h"
#include "SourceImage.h"

#include <QList>
#include <memory>

namespace Fifteen {

class IShuffler;

class GameLikeFifteen : public IGame
{
    Q_OBJECT
public:
    GameLikeFifteen(const SourceImage &sourceImg, const QSize &xy, IShuffler *shuffler);
    GameLikeFifteen(IShuffler *shuffler);
    ~GameLikeFifteen() = default;

    // IGame
    GameID gameID() const override;

    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    SourceImage sourceImage() const override;

protected:
    virtual void click(const QPoint &posInArray) = 0;

    bool isGameCleared() const;
    void saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const;

    GameID gameId;
    QList<QList<PuzzlePiecePointer>> pieces;
    QList<QPoint> changedPos;

    SourceImage sourceImg;
    QSize xy;

    bool isStarted;

    QList<double> splitterXs;
    QList<double> splitterYs;

private slots:
    void piecesUpdated(QList<QPoint> &changedPos);

private:
    void drawAll();
    void drawChanged();
    void drawPiece(QPainter &painterBuffer, const QPoint &posInArray) const;
    void calcSplitterPos();

    QPixmap backBuffer;
    std::unique_ptr<IShuffler> shuffler;
};

} // Fifteen

#endif // GAMELIKEFIFTEEN_H