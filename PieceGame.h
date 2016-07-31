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
#ifndef PIECEGAME_H
#define PIECEGAME_H

#include "IGame.h"
#include "IGameData.h"
#include "GameID.h"

#include <QList>
#include <QPixmap>

class PieceGame : public IGame
{
    Q_OBJECT
public:
    PieceGame(GameDataPointer gameData);
    ~PieceGame() = default;

    GameID gameID() const override;

    IGame *cloneAsNewGame() const override;
    void save(const QString &saveDirPath, const QSize &screenshotSize) const override;
    bool load(const QString &loadFilePath) override;

    void onTickFrame() override;
    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void draw(QPainter &dest) override;
    QSize maxFieldSize() const override;
    void drawFinalImage(QPainter &dest) const override;
    QString shortInformation() const override;
    const SourceImage &sourceImage() const override;

protected slots:
    void changePhase(IPhase::PhaseType phaseType);

protected:
    void saveScreenshot(const QString &saveDirPath, const QSize &screenshotSize) const override;

private:
    GameID gameId;

    QPixmap backBuffer;

    GameDataPointer gameData;
    PhasePointer phase;
};

#endif // GAMECORE_H
