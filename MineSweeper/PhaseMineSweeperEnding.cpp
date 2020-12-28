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
#include "PhaseMineSweeperEnding.h"
#include "AnimationObject/Effect/EffectGraduallyImage.h"
#include <QDebug>

namespace MineSweeper {

PhaseMineSweeperEnding::PhaseMineSweeperEnding(BoardInfoPointer boardInfo, QVector<QVector<MinePiecePointer>> &pieces, SourceImage sourceImage, PhaseType nextPhase) :
    pieces(pieces),
    sourceImage(sourceImage),
    nextPhase(nextPhase),
    nowFrame(0),
    mt(std::random_device()())
{
    Q_CHECK_PTR(boardInfo);
    Q_ASSERT(!sourceImage.isNull());

    setEffectToPieces(boardInfo);
}

void PhaseMineSweeperEnding::onTickFrame()
{
    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->onTickFrame();
    }

    if (++nowFrame > (maxWaitFrame + eraseFrames * 3))
        toNextPhase(nextPhase);
}

void PhaseMineSweeperEnding::draw(QPainter &painter)
{
    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->draw(painter);
    }

    double opacity = qMin(1.0, static_cast<double>(nowFrame - (maxWaitFrame + eraseFrames)) / (eraseFrames * 2) / 10);

    if (opacity > 0) {
        painter.save();
        painter.setOpacity(opacity);
        painter.drawPixmap(painter.viewport(), sourceImage.pixmap());
        painter.restore();
    }
}

bool PhaseMineSweeperEnding::canSave() const
{
    return false;
}

bool PhaseMineSweeperEnding::canLoad() const
{
    return false;
}

QString PhaseMineSweeperEnding::information() const
{
    return "Cleared!";
}

void PhaseMineSweeperEnding::setEffectToPieces(BoardInfoPointer boardInfo)
{
    Q_ASSERT(pieces.size() > 1);

    for (int y = 1, ylim = pieces.size() - 1; y < ylim; ++y) {
        auto &horizontal = pieces[y];

        Q_ASSERT(horizontal.size() > 1);

        for (int x = 1, xlim = horizontal.size() - 1; x < xlim; ++x) {
            auto &piece = horizontal[x];

            if (piece->isMine()) {
                auto rect = boardInfo->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect();
                piece->setEffect(std::make_shared<Effect::GraduallyImage>(mt() % maxWaitFrame, eraseFrames, sourceImage.pixmap(), rect));
//            } else if (piece->isNearMine()) {
//                auto rect = boardInfo->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect();
//                piece->setEffect(std::make_shared<Effect::GraduallyImage>(maxWaitFrame + totalEraseFrames, totalEraseFrames, sourceImage.pixmap, rect));
            }
        }
    }
}

} // MineSweeper
