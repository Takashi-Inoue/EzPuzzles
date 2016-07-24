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
#include "GameSimpleSlide.h"
#include "SimplePiecesFactory.h"
#include "SlideBlankPiece.h"
#include "FifteenSlideShuffler.h"
#include "FifteenPieceMover.h"
#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

extern QThread gameThread;

namespace Fifteen {

QString GameSimpleSlide::gameName()
{
    return "Simple Slide";
}

GameSimpleSlide::GameSimpleSlide(const SourceImage &sourceImg, const QSize &xy, const UniquePosition &blankPos) :
    GameLikeFifteen(sourceImg, xy),
    slideAnimationFrames(10),
    defaultBlankPos(blankPos),
    blankPos(blankPos.selectedPosition()),
    isRandomBlank(isRandomBlank)
{
    Q_ASSERT(!sourceImg.isNull());
    Q_ASSERT(!xy.isEmpty());
    Q_ASSERT(this->blankPos.x() >= 0 && this->blankPos.x() < xy.width());
    Q_ASSERT(this->blankPos.y() >= 0 && this->blankPos.y() < xy.height());

    initPieces();
    initBlankPiece();
    setAnimationToPieces();
    createShuffler();
}

IGame *GameSimpleSlide::cloneAsNewGame() const
{
    auto game = new GameSimpleSlide(sourceImg, boardInfo->boardSize(), defaultBlankPos);

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void GameSimpleSlide::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QFile file(saveDirPath + "/" + gameId.toString() + ".dat");

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    // 以下の書き込み順はこの2関数に影響するので注意すること
    // GameSimpleSlide::load()
    // SimpleSlideGameInfo::load()

    QDataStream stream(&file);

    stream << gameName();
    stream << sourceImg.fullPath;
    stream << boardInfo->boardSize();
    defaultBlankPos.write(stream);
    stream << blankPos;
    stream << static_cast<qint8>(gamePhase);
    stream << sourceImg.pixmap;

    QList<QPoint> defaultPositions;

    for (const auto &piece : pieces)
        defaultPositions << piece->pos().defaultPos();

    stream << defaultPositions;

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

    gameId = GameID::fromQString(QFileInfo(loadFilePath).baseName());

    QSize xy;
    qint8 phase;

    stream >> sourceImg.fullPath;
    stream >> xy;
    defaultBlankPos.read(stream);
    stream >> blankPos;
    stream >> phase;
    stream >> sourceImg.pixmap;

    gamePhase = static_cast<GamePhase>(phase);

    boardInfo = std::make_shared<BoardInformation>(xy, sourceImg.size());

    QList<QPoint> defaultPositions;
    stream >> defaultPositions;

    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces(defaultPositions);

    initBlankPiece();
    setAnimationToPieces();

    createShuffler();

    createBackBuffer();
    drawAllPieces();

    return true;
}

QString GameSimpleSlide::shortInformation() const
{
    return gameName();
}

void GameSimpleSlide::click(const QPoint &piecePos)
{
    if (piecePos == blankPos)
        return;

    if ((piecePos.x() != blankPos.x()) & (piecePos.y() != blankPos.y()))
        return;

    addChangedPieces(piecePos.x() == blankPos.x() ? PieceMover(pieces, boardInfo->xCount()).slideVertical(blankPos, piecePos)
                                                  : PieceMover(pieces, boardInfo->xCount()).slideHorizontal(blankPos, piecePos)
                    );

    blankPos = piecePos;
}

void GameSimpleSlide::createShuffler()
{
    Q_ASSERT(boardInfo != nullptr);

    shuffler  = std::make_unique<SlideShuffler>(pieces, boardInfo, blankPos);

    connect(this, SIGNAL(startShuffle()), shuffler.get(), SLOT(start()));
    connect(shuffler.get(), SIGNAL(completed()), this, SLOT(onCompletedShuffling()));
    connect(shuffler.get(), SIGNAL(update(QList<PuzzlePiecePointer>)), this, SLOT(addChangedPieces(QList<PuzzlePiecePointer>)));

    shuffler->moveToThread(&gameThread);
}

void GameSimpleSlide::initPieces()
{
    pieces = SimplePiecesFactory(boardInfo, sourceImg.pixmap).createPieces();
}

void GameSimpleSlide::initBlankPiece()
{
    auto &blankPiece = pieces[blankPos.y() * boardInfo->xCount() + blankPos.x()];
    blankPiece = std::make_shared<SlideBlankPiece>(boardInfo, defaultBlankPos.selectedPosition(), Qt::black, slideAnimationFrames);
    blankPiece->setPosWithoutAnimation(blankPos);

    addChangedPieces({blankPiece});
}

void GameSimpleSlide::setAnimationToPieces()
{
    auto frame = std::make_shared<Effect::SimpleFrame>(2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192));

    for (auto &piece : pieces) {
        piece->setAnimation(std::make_shared<Animation::LineMove>(slideAnimationFrames, false));
        piece->setEffect(frame);
    }

    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(4, QColor(0, 0, 0), QColor(0, 0, 0), QColor(64, 192, 224, 224), QColor(16, 64, 96, 224), 120, true);

    pieces[blankPos.y() * boardInfo->xCount() + blankPos.x()]->setEffect(graduallyFrame);
}

GameSimpleSlide::GameSimpleSlide() :
    slideAnimationFrames(10)
{
}

} // Fifteen
