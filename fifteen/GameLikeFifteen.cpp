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
#include "GridSplitter.h"
#include "FifteenIShuffler.h"

namespace Fifteen {

GameLikeFifteen::GameLikeFifteen(const SourceImage &sourceImg, const QSize &xy, IShuffler *shuffler) :
    sourceImg(sourceImg),
    xy(xy),
    isStarted(false),
    backBuffer(sourceImg.size()),
    shuffler(shuffler)
{
    Q_ASSERT(!sourceImg.pixmap.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(shuffler != nullptr);

    connect(shuffler, SIGNAL(update(QList<QPoint>&)), this, SLOT(piecesUpdated(QList<QPoint>&)));
}

GameLikeFifteen::GameLikeFifteen(IShuffler *shuffler) :
    shuffler(shuffler)
{
    Q_ASSERT(shuffler != nullptr);
}

GameID GameLikeFifteen::gameID() const
{
    return gameId;
}

void GameLikeFifteen::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    if (!isStarted) {
        isStarted = true;
        shuffler->shufflePieces();

        return;
    }

    QSize fieldCellSize(fieldSize.width() / xy.width(), fieldSize.height() / xy.height());
    QPoint posInArray(cursorPos.x() / fieldCellSize.width(), cursorPos.y() / fieldCellSize.height());

    click(posInArray);

    if (isGameCleared())
        isStarted = false;

    emit screenUpdated();
}

void GameLikeFifteen::draw(QPainter &dest)
{
    if (splitterXs.isEmpty())
        calcSplitterPos();

    if (backBuffer.isNull()) {
        backBuffer = QPixmap(sourceImg.size());

        if (isStarted) {
            drawAll();
            GridSplitter(QPen(Qt::darkGray, 1), xy.width() - 1, xy.height() - 1).draw(QPainter(&backBuffer));
        }
    }

    if (!isStarted) {
        drawFinalImage(QPainter(&backBuffer));
    } else {
        drawChanged();

        if (!changedPos.isEmpty())
            GridSplitter(QPen(Qt::darkGray, 1), xy.width() - 1, xy.height() - 1).draw(QPainter(&backBuffer));
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
    for (int y = 0, ylim = xy.height(); y < ylim; ++y) {
        for (int x = 0, xlim = xy.width(); x < xlim; ++x) {
            if (!pieces.at(y).at(x)->isPosCorrect())
                return false;
        }
    }

    return true;
}

void GameLikeFifteen::saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = saveDirPath + "/" + gameId.toString() + ".png";
    backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(ssPath, "PNG");
}

void GameLikeFifteen::piecesUpdated(QList<QPoint> &changedPos)
{
    this->changedPos = changedPos;

    emit screenUpdated();
}

void GameLikeFifteen::drawAll()
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (int y = 0, ylim = xy.height(); y < ylim; ++y) {
        for (int x = 0, xlim = xy.width(); x < xlim; ++x) {
            drawPiece(painterBuffer, QPoint(x, y));
        }
    }
}

void GameLikeFifteen::drawChanged()
{
    QPainter painterBuffer(&backBuffer);

    painterBuffer.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (auto &posInArray : changedPos)
        drawPiece(painterBuffer, posInArray);
}

void GameLikeFifteen::drawPiece(QPainter &painterBuffer, const QPoint &posInArray) const
{
    QPointF destTL(splitterXs.at(posInArray.x()    ), splitterYs.at(posInArray.y()    ));
    QPointF destBR(splitterXs.at(posInArray.x() + 1), splitterYs.at(posInArray.y() + 1));
    QRectF destRect(destTL, destBR);

    pieces.at(posInArray.y()).at(posInArray.x())->draw(painterBuffer, destTL, destRect.size());
}

void GameLikeFifteen::calcSplitterPos()
{
    splitterXs.push_back(0);
    splitterYs.push_back(0);

    splitterXs << GridSplitter::verticalSplitterPos(sourceImg.width(), xy.width() - 1) << sourceImg.width();
    splitterYs << GridSplitter::horizontalSplitterPos(sourceImg.height(), xy.height() - 1) << sourceImg.height();
}

} // Fifteen
