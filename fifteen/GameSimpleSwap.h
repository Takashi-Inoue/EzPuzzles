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
#ifndef GAMESIMPLESWAP_H
#define GAMESIMPLESWAP_H

#include "GameLikeFifteen.h"

namespace Fifteen {

class GameSimpleSwap : public GameLikeFifteen
{
    Q_OBJECT
public:
    static QString gameName();

    GameSimpleSwap(const SourceImage &sourceImg, const QSize &xy, const QPoint &swapTargetPos);

    // IGame
    IGame *cloneAsNewGame() const override;
    void save(const QString &saveDirPath, const QSize &screenshotSize) const override;
    bool load(const QString &loadFilePath) override;

    QString shortInformation() const override;

protected:
    // GameLikeFifteen
    void click(const QPoint &piecePos) override;

    void initPieces();

    QPoint swapTargetPos;

private:
    friend class SimpleSwapSaveData;
    GameSimpleSwap();
};

} // Fifteen

#endif // GAMESIMPLESWAP_H
