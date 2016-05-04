/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "GameSimpleSlide.h"
#include "SimplePiecesFactory.h"
#include "FifteenSlideShuffler.h"
#include "FifteenPieceMover.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

namespace Fifteen {

QString GameSimpleSlide::gameName()
{
    return "Simple Slide";
}

GameSimpleSlide::GameSimpleSlide(const SourceImage &sourceImg, const QSize &xy, const QPoint &blankPos) :
    GameLikeFifteen(sourceImg, xy, new SlideShuffler(pieces, this->blankPos)),
    blankPos(blankPos)
{
    Q_ASSERT(!sourceImg.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(blankPos.x() >= 0 && blankPos.x() < xy.width());
    Q_ASSERT(blankPos.y() >= 0 && blankPos.y() < xy.height());

    initPieces();
    initBlankPiece();
}

IGame *GameSimpleSlide::cloneAsNewGame() const
{
    return nullptr;
}

void GameSimpleSlide::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameID.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << xy;
    stream << isStarted;
    stream << blankPos;
    stream << sourceImg.pixmap;

    saveScreenshot(saveDirPath, screenshotSize);
}

bool GameSimpleSlide::load(const QString &loadFilePath)
{
    QFile file(loadFilePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);

    QString name;

    stream >> name;

    if (name != gameName())
        return false;

    gameID = GameID::fromQString(QFileInfo(loadFilePath).baseName());

    QString sourceImgFullPath;
    QPixmap sourceImgPixmap;

    stream >> sourceImg.fullPath;
    stream >> xy;
    stream >> isStarted;
    stream >> blankPos;
    stream >> sourceImg.pixmap;

    initPieces();

    // ここでpiece読み込み

    initBlankPiece();

    return true;
}

QString GameSimpleSlide::shortInformation() const
{
    return gameName();
}

void GameSimpleSlide::click(const QPoint &posInArray)
{
    if (posInArray == blankPos)
        return;

    if ((posInArray.x() != blankPos.x()) & (posInArray.y() != blankPos.y()))
        return;

    changedPos = posInArray.x() == blankPos.x() ? PieceMover(pieces).slideVertical(blankPos, posInArray)
                                                : PieceMover(pieces).slideHorizontal(blankPos, posInArray);

    blankPos = posInArray;
}

void GameSimpleSlide::initPieces()
{
    pieces = SimplePiecesFactory(sourceImg.pixmap, xy).createPieces();
}

void GameSimpleSlide::initBlankPiece()
{
    QPixmap pixmapBlank(1, 1);
    pixmapBlank.fill(Qt::black);

    pieces[blankPos.y()][blankPos.x()] = SimplePiecesFactory::createPiece(blankPos, pixmapBlank);
}

} // Fifteen
