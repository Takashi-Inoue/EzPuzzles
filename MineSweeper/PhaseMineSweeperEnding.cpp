﻿/*
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

#include "AnimationFactory.h"
#include "AnimationObject/Effect/ChainedEffect.h"

#include <QRandomGenerator>
#include <QDebug>

namespace MineSweeper {

PhaseMineSweeperEnding::PhaseMineSweeperEnding(BoardInfoPointer boardInfo, QVector<QVector<MinePiecePointer>> &pieces, SourceImage sourceImage, PhaseType nextPhase, QObject *parent) :
    AbstractPhase(nextPhase, parent),
    pieces(pieces),
    sourceImage(sourceImage),
    nowFrame(0)
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
        toNextPhase(m_nextPhaseType);
}

void PhaseMineSweeperEnding::draw(QPainter &painter)
{
    for (auto &horizontal : pieces) {
        for (auto &piece : horizontal)
            piece->draw(painter);
    }

    double opacity = qMin(1.0, double(nowFrame - (maxWaitFrame + eraseFrames)) / (eraseFrames * 2) / 10);

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
    return QStringLiteral("Cleared!");
}

void PhaseMineSweeperEnding::setEffectToPieces(BoardInfoPointer boardInfo)
{
    Q_ASSERT(pieces.size() > 1);

    QRandomGenerator *randomGenerator = QRandomGenerator::global();

    for (int y = 1, ylim = int(pieces.size() - 1); y < ylim; ++y) {
        auto &horizontal = pieces[y];

        Q_ASSERT(horizontal.size() > 1);

        for (int x = 1, xlim = int(horizontal.size() - 1); x < xlim; ++x) {
            auto &piece = horizontal[x];

            if (piece->isMine()) {
                auto chainedEffect = QSharedPointer<Effect::ChainedEffect>::create(false);

                int waitFrames = randomGenerator->bounded(maxWaitFrame);
                chainedEffect->addEffect(AnimationFactory::waitEffect(waitFrames));

                QRectF rect = boardInfo->rectFromPiecePos(QPoint(x - 1, y - 1));
                EffectPointer effect = AnimationFactory::graduallyImage(eraseFrames
                                         , Qt::transparent, sourceImage.pixmap(), rect);

                chainedEffect->addEffect(effect);
                piece->setEffect(chainedEffect);
//            } else if (piece->isNearMine()) {
//                auto rect = boardInfo->rectFromPiecePos(QPoint(x - 1, y - 1)).toRect();
//                piece->setEffect(QSharedPointer<Effect::GraduallyImage>::create(maxWaitFrame + eraseFrames, eraseFrames, sourceImage.pixmap(), rect));
            }
        }
    }
}

} // MineSweeper
