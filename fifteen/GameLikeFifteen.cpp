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
#include "GameLikeFifteen.h"
#include "FifteenAbstractShuffler.h"

#include <QThread>
#include <QElapsedTimer>
#include <QDebug>

namespace Fifteen {

GameLikeFifteen::GameLikeFifteen(const SourceImage &sourceImg, const QSize &xy, AbstractShuffler *shuffler) :
    boardInfo(std::make_shared<BoardInformation>(xy, sourceImg.size())),
    sourceImg(sourceImg),
    gamePhase(PhaseReady),
    backBuffer(sourceImg.size()),
    shuffler(shuffler)
{
    Q_ASSERT(!sourceImg.pixmap.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(shuffler != nullptr);

    createBackBuffer();
    drawFinalImage(QPainter(&backBuffer));

    connect(shuffler, SIGNAL(update(QList<PuzzlePiecePointer>)), this, SLOT(drawPieces(QList<PuzzlePiecePointer>)));
}

GameLikeFifteen::GameLikeFifteen(AbstractShuffler *shuffler) : // protected constructor
    shuffler(shuffler)
{
    Q_ASSERT(shuffler != nullptr);

    connect(shuffler, SIGNAL(update(QList<PuzzlePiecePointer>)), this, SLOT(drawPieces(QList<QPoint>)));
}

GameID GameLikeFifteen::gameID() const
{
    return gameId;
}

void GameLikeFifteen::onTickFrame()
{
    for (auto &piece : changedPieces)
        piece->onTickFrame();
}

void GameLikeFifteen::click(const QSize &fieldSize, const QPoint &cursorPos)
{
//    qDebug() << thread()->currentThreadId();

    if ((gamePhase == PhasePreGaming) | (gamePhase == PhaseEnding))
        return;

    if (gamePhase == PhaseReady) {
        gamePhase = PhasePreGaming;

        drawAllPieces();
        shuffler->shufflePieces();

        gamePhase = PhaseGaming;

        return;
    }

    for (auto &piece : changedPieces)
        piece->skipAnimation();

    double mag = static_cast<double>(backBuffer.width()) / fieldSize.width();

    click(boardInfo->piecePosFromPixelPos(cursorPos * mag));

    if (isGameCleared())
        gamePhase = PhaseEnding;
}

void GameLikeFifteen::draw(QPainter &dest)
{
    if ((gamePhase == PhaseGaming) | (gamePhase == PhaseEnding)) {
        drawPieces(changedPieces);

        auto itr = std::remove_if(changedPieces.begin(), changedPieces.end(), [](PuzzlePiecePointer &piece){
            return piece->animation()->isFinishedAnimation() && piece->effect()->isFinishedAnimation();
        });

        changedPieces.erase(itr, changedPieces.end());
    }

    dest.drawPixmap(dest.viewport(), backBuffer, backBuffer.rect());
}

QSize GameLikeFifteen::maxFieldSize() const
{
    return sourceImg.size();
}

void GameLikeFifteen::drawFinalImage(QPainter &dest) const
{
    QSize destSize = sourceImg.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio);
    dest.drawPixmap(QRect(QPoint(0, 0), destSize), sourceImg.pixmap, sourceImg.rect());
}

SourceImage GameLikeFifteen::sourceImage() const
{
    return sourceImg;
}

bool GameLikeFifteen::isGameCleared() const
{
    for (const auto &piece : pieces) {
        if (!piece->pos().isCorrect())
            return false;
    }

    return true;
}

void GameLikeFifteen::createBackBuffer()
{
    if (!backBuffer.isNull())
        return;

    backBuffer = QPixmap(sourceImg.size());
}

void GameLikeFifteen::drawAllPieces()
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (auto &piece : pieces)
        piece->draw(painterBuffer);
}

void GameLikeFifteen::saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = saveDirPath + "/" + gameId.toString() + ".png";
    backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(ssPath, "PNG");
}

void GameLikeFifteen::addChangedPieces(QList<PuzzlePiecePointer> changed)
{
    changedPieces += changed;

    qSort(changedPieces);

    changedPieces.erase(std::unique(changedPieces.begin(), changedPieces.end()), changedPieces.end());
}

void GameLikeFifteen::drawPieces(const QList<PuzzlePiecePointer> &drawPieces)
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (auto &piece : drawPieces)
        piece->draw(painterBuffer);
}

} // Fifteen
