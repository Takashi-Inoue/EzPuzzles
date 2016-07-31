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
#ifndef GAMELIKEFIFTEEN_H
#define GAMELIKEFIFTEEN_H

#include "IGame.h"
#include "IPuzzlePiece.h"
#include "GameID.h"
#include "SourceImage.h"
#include "BoardInformation.h"

#include <QList>
#include <QReadWriteLock>
#include <memory>

namespace Fifteen {

class AbstractShuffler;

class GameLikeFifteen : public IGame
{
    Q_OBJECT
public:
    GameLikeFifteen(const SourceImage &sourceImg, const QSize &xy);
    ~GameLikeFifteen() = default;

    // IGame
    GameID gameID() const override;

    void onTickFrame() override;
    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    const SourceImage &sourceImage() const override;

signals:
    void startShuffle();

protected:
    enum GamePhase {
        PhaseReady,
        PhasePreGaming,
        PhaseGaming,
        PhaseEnding,
        PhaseCleared,
    };

    GameLikeFifteen() = default;
    virtual void click(const QPoint &posInArray) = 0;

    bool isGameCleared() const;
    void createBackBuffer();
    void drawAllPieces();
    void saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const;
    PuzzlePiecePointer &getPiece(const QPoint &piecePos);

    GameID gameId;
    BoardInfoPointer boardInfo;

    QList<PuzzlePiecePointer> pieces;
    SourceImage sourceImg;
    GamePhase gamePhase;

protected slots:
    void onCompletedShuffling();
    void addChangedPieces(QList<PuzzlePiecePointer> changed);
    void drawPieces(const QList<PuzzlePiecePointer> &drawPieces);

private:
    QList<PuzzlePiecePointer> changedPieces;

    QPixmap backBuffer;
};

} // Fifteen

#endif // GAMELIKEFIFTEEN_H
