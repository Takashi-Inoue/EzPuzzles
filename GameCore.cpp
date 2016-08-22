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
#include "GameCore.h"
#include "SourceImage.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

GameCore::GameCore(GameDataPointer gameData) :
    gameData(gameData),
    backBuffer(QPixmap(gameData->boardInfo()->boardPixelSize()))
{
    Q_CHECK_PTR(gameData);

    changePhase(gameData->currentPhase());
}

GameCore::GameCore(GameDataPointer gameData, GameID id) :
    gameData(gameData),
    gameId(id),
    backBuffer(QPixmap(gameData->boardInfo()->boardPixelSize()))
{
    Q_CHECK_PTR(gameData);

    changePhase(gameData->currentPhase());
}

GameID GameCore::gameID() const
{
    return gameId;
}

IGame *GameCore::cloneAsNewGame() const
{
    auto game = new GameCore(gameData->cloneAsNewGame());

    const_cast<GameID *>(&gameId)->swap(*const_cast<GameID *>(&game->gameId));

    return game;
}

void GameCore::save(const QString &saveDirPath, const QSize &screenshotSize) const
{
    if (!phase->canSave())
        return;

    QString fileName = saveDirPath + "/" + gameId.toString() + ".dat";

    gameData->save(fileName);

    saveScreenshot(saveDirPath, screenshotSize);
}

void GameCore::onTickFrame()
{
    Q_CHECK_PTR(phase);

    phase->onTickFrame();
}

void GameCore::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    phase->click(piecePosFromCursorPos(fieldSize, cursorPos));
}

void GameCore::draw(QPainter &dest)
{
    QPainter painter(&backBuffer);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    phase->draw(painter);

    dest.save();
    dest.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    dest.drawPixmap(dest.viewport(), backBuffer);
    dest.restore();
}

QSize GameCore::maxFieldSize() const
{
    return backBuffer.size();
}

void GameCore::drawFinalImage(QPainter &dest) const
{
    auto &finalImage = gameData->finalImage();

    if (finalImage != nullptr)
        finalImage->draw(dest);
}

QString GameCore::shortInformation() const
{
    return gameData->gameName() + " - " + phase->information();
}

const SourceImage &GameCore::sourceImage() const
{
    return gameData->sourceImage();
}

void GameCore::changePhase(IPhase::PhaseType phaseType)
{
    qDebug() << "changePhase" << phaseType;

    if (phase != nullptr)
        phase->disconnect();

    phase = gameData->createPhase(phaseType);

    connect(phase.get(), SIGNAL(toNextPhase(IPhase::PhaseType)), this, SLOT(changePhase(IPhase::PhaseType)));

    emit informationUpdated();
}

void GameCore::saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = saveDirPath + "/" + gameId.toString() + ".png";
    backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(ssPath, "PNG");
}

QPoint GameCore::piecePosFromCursorPos(const QSize &fieldSize, const QPoint &cursorPos) const
{
    const auto &boardInfo = gameData->boardInfo();

    double scale = static_cast<double>(boardInfo->boardPixelSize().width()) / fieldSize.width();

    return boardInfo->piecePosFromPixelPos(cursorPos * scale);
}
