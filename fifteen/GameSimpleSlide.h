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
#ifndef GAMESIMPLESLIDE_H
#define GAMESIMPLESLIDE_H

#include "GameLikeFifteen.h"

namespace Fifteen {

class GameSimpleSlide : public GameLikeFifteen
{
    Q_OBJECT
public:
    static QString gameName();

    GameSimpleSlide(const SourceImage &sourceImg, const QSize &xy, const QPoint &blankPos);

    // IGame
    IGame *cloneAsNewGame() const override;
    void save(const QString &saveDirPath, const QSize &screenshotSize) const override;
    bool load(const QString &loadFilePath) override;

    QString shortInformation() const override;

protected:
    // GameLikeFifteen
    void click(const QPoint &piecePos) override;

    void initPieces();
    void initBlankPiece();

    void setAnimationToPieces();

    int slideAnimationFrames;

    QPoint defaultBlankPos;
    QPoint blankPos;

private:
    friend class SimpleSlideSaveData;
    GameSimpleSlide();
};

} // Fifteen

#endif // GAMESIMPLESLIDE_H
