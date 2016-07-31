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
#include "PhaseSimpleSlideGaming.h"
#include "fifteen/FifteenPieceMover.h"

PhaseSimpleSlideGaming::PhaseSimpleSlideGaming(QList<Fifteen::PuzzlePiecePointer> &pieces, QPoint &currentBlankPos, PhaseType nextPhase, QObject *parent) :
    IPhase(parent),
    pieces(pieces),
    blankPos(currentBlankPos),
    nextPhase(nextPhase)
{
}

void PhaseSimpleSlideGaming::click(const QPoint &clickPiecePos)
{
    if (clickPiecePos == blankPos)
        return;

    if ((clickPiecePos.x() != blankPos.x()) & (clickPiecePos.y() != blankPos.y()))
        return;



    blankPos = clickPiecePos;

    if (isCleared())
        emit toNextPhase(nextPhase);
}

void PhaseSimpleSlideGaming::onTickFrame()
{
    for (auto &piece : animationPieces)
        piece->onTickFrame();
}

void PhaseSimpleSlideGaming::draw(QPainter &painter)
{
    for (auto &piece : animationPieces)
        piece->draw(painter);

    auto itr = std::remove_if(animationPieces.begin(), animationPieces.end(), [](Fifteen::PuzzlePiecePointer &piece) {
        bool f1 = (piece->animation() == nullptr || piece->animation()->isFinishedAnimation());
        bool f2 = (piece->effect() == nullptr || piece->effect()->isFinishedAnimation());

        return (f1 & f2);
    });

    animationPieces.erase(itr, animationPieces.end());
}

bool PhaseSimpleSlideGaming::canSave() const
{
    return true;
}

bool PhaseSimpleSlideGaming::canLoad() const
{
    return true;
}

bool PhaseSimpleSlideGaming::save(QDataStream &) const
{
    return true;
}

bool PhaseSimpleSlideGaming::load(QDataStream &)
{
    return true;
}

QString PhaseSimpleSlideGaming::information() const
{
    return "";
}

bool PhaseSimpleSlideGaming::isCleared() const
{
    for (const auto &piece : pieces) {
        if (!piece->pos().isCorrect())
            return false;
    }

    return true;
}
