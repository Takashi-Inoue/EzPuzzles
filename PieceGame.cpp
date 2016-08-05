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
#include "PieceGame.h"
#include "SourceImage.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

PieceGame::PieceGame(GameDataPointer gameData) :
    backBuffer(QPixmap(gameData->boardInfo()->boardPixelSize())),
    gameData(gameData),
    phase(nullptr)
{
    Q_CHECK_PTR(gameData);

    changePhase(IPhase::PhaseReady);
}

GameID PieceGame::gameID() const
{
    return gameId;
}

IGame *PieceGame::cloneAsNewGame() const
{
    auto game = new PieceGame(gameData);

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void PieceGame::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    if (!phase->canSave())
        return;

    QString fileName = saveDirPath + "/" + gameId.toString() + ".dat";

    gameData->save(fileName);

    saveScreenshot(saveDirPath, screenshotSize);
}

bool PieceGame::load(const QString &loadFilePath)
{
    if (!gameData->load(loadFilePath))
        return false;

    backBuffer = QPixmap(gameData->boardInfo()->boardPixelSize());

    return true;
}

void PieceGame::onTickFrame()
{
    Q_CHECK_PTR(phase);

    phase->onTickFrame();
}

void PieceGame::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    const auto &boardInfo = gameData->boardInfo();

    double scale = static_cast<double>(boardInfo->boardPixelSize().width()) / fieldSize.width();
    QPoint piecePos = boardInfo->piecePosFromPixelPos(cursorPos * scale);

    phase->click(piecePos);
}

void PieceGame::draw(QPainter &dest)
{
    QPainter painter(&backBuffer);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    phase->draw(painter);

    dest.save();
    dest.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    dest.drawPixmap(dest.viewport(), backBuffer);
    dest.restore();
}

QSize PieceGame::maxFieldSize() const
{
    return backBuffer.size();
}

void PieceGame::drawFinalImage(QPainter &dest) const
{
    const auto &sourceImg = gameData->sourceImage();

    QSize destSize = sourceImg.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio);
    QPoint tl((dest.viewport().width() - destSize.width()) / 2, (dest.viewport().height() - destSize.height()) / 2);

    dest.drawPixmap(QRect(tl, destSize), sourceImg.pixmap, sourceImg.rect());
}

QString PieceGame::shortInformation() const
{
    return gameData->gameName() + " - " + phase->information();
}

const SourceImage &PieceGame::sourceImage() const
{
    return gameData->sourceImage();
}

void PieceGame::changePhase(IPhase::PhaseType phaseType)
{
    qDebug() << "changePhase" << phaseType;

    if (phase != nullptr)
        phase->disconnect();

    phase = gameData->createPhase(phaseType);

    connect(phase.get(), SIGNAL(toNextPhase(IPhase::PhaseType)), this, SLOT(changePhase(IPhase::PhaseType)));
}

void PieceGame::saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = saveDirPath + "/" + gameId.toString() + ".png";
    backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(ssPath, "PNG");
}
