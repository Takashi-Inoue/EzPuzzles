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
#ifndef GAMECORE_H
#define GAMECORE_H

#include "IGame.h"
#include "IGameData.h"
#include "GameID.h"

#include <QPixmap>

class GameCore : public IGame
{
    Q_OBJECT
public:
    GameCore(GameDataPointer gameData);
    GameCore(GameDataPointer gameData, GameID id);

    GameID gameID() const override;

    QSharedPointer<IGame> cloneAsNewGame() const override;
    void save(QStringView saveDirPath, const QSize &screenshotSize) const override;

    void onTickFrame() override;

    void click(const QSize &fieldSize, const QPoint &cursorPos) override;

    void draw(QPainter &dest) override;
    void drawFinalImage(QPainter &dest) const override;

    bool hasFinalImage() const override;
    QSize maxFieldSize() const override;
    QString shortInformation() const override;
    const SourceImage &sourceImage() const override;

protected slots:
    void changePhase(AbstractPhase::PhaseType phaseType);

protected:
    void saveScreenshot(QStringView saveDirPath, const QSize &screenshotSize) const override;
    QPoint piecePosFromCursorPos(const QSize &fieldSize, const QPoint &cursorPos) const;

    PhasePointer m_phase;
    GameDataPointer m_gameData;

    GameID m_gameId;

private:
    QPixmap m_backBuffer;
};

#endif // GAMECORE_H
