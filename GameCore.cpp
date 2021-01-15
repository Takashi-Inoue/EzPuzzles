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
#include "GameCore.h"

#include <QDebug>

GameCore::GameCore(GameDataPointer gameData)
    : GameCore(gameData, GameID())
{
}

GameCore::GameCore(GameDataPointer gameData, GameID id) :
    m_gameData(gameData),
    m_gameId(id),
    m_backBuffer(QPixmap(gameData->boardInfo()->boardPixelSize()))
{
    Q_CHECK_PTR(gameData);

    if (hasFinalImage()) {
        connect(gameData->finalImage().get(), &FinalImage::requestRepaint
              , this, &IGame::finalImageUpdated);
    }

    changePhase(gameData->currentPhase());
}

GameID GameCore::gameID() const
{
    return m_gameId;
}

QSharedPointer<IGame> GameCore::cloneAsNewGame() const
{
    auto game = new GameCore(m_gameData->cloneAsNewGame());

    const_cast<GameID *>(&m_gameId)->swap(*const_cast<GameID *>(&game->m_gameId));

    return QSharedPointer<IGame>(game);
}

void GameCore::save(QStringView saveDirPath, const QSize &screenshotSize) const
{
    if (!m_phase->canSave())
        return;

    QString fileName = QStringLiteral("%1/%2.dat").arg(saveDirPath, m_gameId.toString());

    m_gameData->save(fileName);

    saveScreenshot(saveDirPath, screenshotSize);
}

void GameCore::onTickFrame()
{
    Q_CHECK_PTR(m_phase);

    m_phase->onTickFrame();
}

void GameCore::mousePress(const QSize &fieldSize, const QPoint &cursorPos)
{
    m_pressedPiecePos = piecePosFromCursorPos(fieldSize, cursorPos);
}

void GameCore::mouseRelease(const QSize &fieldSize, const QPoint &cursorPos)
{
    QPoint releasedPiecePos = piecePosFromCursorPos(fieldSize, cursorPos);

    if (m_pressedPiecePos == releasedPiecePos) {
        m_phase->click(releasedPiecePos);

        emit informationUpdated(shortInformation());
    }

    m_pressedPiecePos = QPoint(-1, -1);
}

void GameCore::draw(QPainter &dest)
{
    QPainter painter(&m_backBuffer);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_phase->draw(painter);

    dest.save();
    dest.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    dest.drawPixmap(dest.viewport(), m_backBuffer);
    dest.restore();
}

void GameCore::drawFinalImage(QPainter &dest) const
{
    FinalImagePointer finalImage = m_gameData->finalImage();

    if (finalImage != nullptr)
        finalImage->draw(dest);
}

bool GameCore::hasFinalImage() const
{
    return true;
}

QSize GameCore::maxFieldSize() const
{
    return m_backBuffer.size();
}

QString GameCore::shortInformation() const
{
    return QStringLiteral("%1 - %2").arg(m_gameData->gameName(), m_phase->information());
}

const SourceImage &GameCore::sourceImage() const
{
    return m_gameData->sourceImage();
}

void GameCore::changePhase(AbstractPhase::PhaseType phaseType)
{
    qDebug() << "changePhase" << phaseType;

    if (m_phase != nullptr)
        m_phase->disconnect();

    m_phase = m_gameData->createPhase(phaseType);

    connect(m_phase.get(), &AbstractPhase::toNextPhase, this, &GameCore::changePhase);

    emit informationUpdated(shortInformation());
}

void GameCore::saveScreenshot(QStringView saveDirPath, const QSize &screenshotSize) const
{
    QString ssPath = QStringLiteral("%1/%2.png").arg(saveDirPath, m_gameId.toString());
    m_backBuffer.scaled(screenshotSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            .save(ssPath, "PNG");
}

QPoint GameCore::piecePosFromCursorPos(const QSize &fieldSize, const QPoint &cursorPos) const
{
    BoardInfoPointer boardInfo = m_gameData->boardInfo();

    double scale = double(boardInfo->boardPixelSize().width()) / fieldSize.width();

    return boardInfo->piecePosFromPixelPos(cursorPos * scale);
}
