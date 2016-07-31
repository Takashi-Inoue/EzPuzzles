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
#ifndef PHASESIMPLESLIDEGAMING_H
#define PHASESIMPLESLIDEGAMING_H

#include "IPhase.h"
#include "fifteen/PuzzlePiece.h"

class PhaseSimpleSlideGaming : public IPhase
{
    Q_OBJECT
public:
    PhaseSimpleSlideGaming(QList<Fifteen::PuzzlePiecePointer> &pieces, QPoint &currentBlankPos, PhaseType nextPhase, QObject *parent = 0);
    ~PhaseSimpleSlideGaming() = default;

    void click(const QPoint &) override;
    void onTickFrame() override;
    void draw(QPainter &) override;
    bool canSave() const override;
    bool canLoad() const override;
    bool save(QDataStream &) const override;
    bool load(QDataStream &) override;
    QString information() const override;

protected:
    bool isCleared() const;

    QList<Fifteen::PuzzlePiecePointer> &pieces;
    QList<Fifteen::PuzzlePiecePointer> animationPieces;
    QPoint &blankPos;
    PhaseType nextPhase;
};

#endif // PHASESIMPLESLIDEGAMING_H
