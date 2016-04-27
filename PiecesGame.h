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
#ifndef PIECESGAME_H
#define PIECESGAME_H

#include "IGame.h"
#include "SourceImage.h"

#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <memory>

class IShuffler;

class PiecesGame : public IGame
{
    Q_OBJECT
public:
    PiecesGame(const SourceImage &sourceImage, const QSize &xyCount, IShuffler *shuffler, QObject *parent = 0);
    ~PiecesGame() = default;

    void save(const QString &savePath) const {}
    bool load(const QString &savePath) {return false;}

    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    SourceImage sourceImage() const override;

protected:
    virtual void clickOperation(const QSize &fieldSize, const QPoint &cursorPos);
    virtual void drawPiece(QPainter &painterBuffer, int pieceIndex);

    bool isGameClear() const;
    void gameOver();
    void drawBlackPiece(QPainter &painterBuffer, int pieceIndex);

    QList<int> pieces;
    const QSize xy;
    QList<int> changedIndex;

private slots:
    void pieceChanged(const QList<int> &changed);

private:
    void drawAll();
    void drawChanged();
    void calcSourceSplitterPos();
    void calcDestSplitterPos();

    SourceImage sourceImg;
    QPixmap backBuffer;

    bool isStarted;

    QList<double> sourceSplitterXs;
    QList<double> sourceSplitterYs;
    QList<double> destSplitterXs;
    QList<double> destSplitterYs;

    std::shared_ptr<IShuffler> shuffler;
};

#endif // PIECESGAME_H
