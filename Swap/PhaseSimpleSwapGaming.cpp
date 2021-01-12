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
#include "PhaseSimpleSwapGaming.h"
#include "Fifteen/FifteenPieceMover.h"
#include "AnimationObject/Effect/CompositeEffect.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "Fifteen/EffectSwapper.h"

namespace Swap {

PhaseSimpleSwapGaming::PhaseSimpleSwapGaming(BoardPointer board, const QPoint &swapTargetPos
                                           , PhaseType nextPhase, int totalMoveFrame, QObject *parent)
    : AbstractPhase(nextPhase, parent)
    , m_swapTargetPos(swapTargetPos)
    , m_board(board)
    , m_totalMoveFrame(totalMoveFrame)
{
    Q_ASSERT(totalMoveFrame >= 0);

    auto graduallyFrame = QSharedPointer<Effect::GraduallyBlinkFrame>::create(
                              8
                            , QColor("#E0FF8040"), QColor("#00FFFF40")
                            , QColor("#E0FFFF40"), QColor("#00FF8040"), 240, true);

    auto compositeEffect = QSharedPointer<Effect::CompositeEffect>::create();

    FifteenPiecePointer &piece = board->piece(swapTargetPos);

    compositeEffect->addEffect(piece->effect());
    compositeEffect->addEffect(graduallyFrame);

    piece->setEffect(compositeEffect);
}

void PhaseSimpleSwapGaming::click(const QPoint &clickedPiecePos)
{
    if (m_isGameCleared | (clickedPiecePos == m_swapTargetPos))
        return;

    m_board->swapPiece(m_swapTargetPos, clickedPiecePos);

    m_effects << QSharedPointer<Fifteen::EffectSwapper>::create(
                           m_board->piece(m_swapTargetPos), m_board->piece(clickedPiecePos)
                         , m_totalMoveFrame / 2);

    m_isGameCleared = m_board->isCleared();
}

void PhaseSimpleSwapGaming::onTickFrame()
{
    m_board->onTickFrame();

    auto itr = std::remove_if(m_effects.begin(), m_effects.end()
                            , [](QSharedPointer<IAnimationObject> &animation)
    {
        return !animation->onTickFrame();
    });

    m_effects.erase(itr, m_effects.end());

    if (m_isGameCleared) {
        if (--m_totalMoveFrame == 0)
            emit toNextPhase(m_nextPhaseType);
    }
}

void PhaseSimpleSwapGaming::draw(QPainter &painter)
{
    m_board->draw(painter);
}

bool PhaseSimpleSwapGaming::canSave() const
{
    return true;
}

bool PhaseSimpleSwapGaming::canLoad() const
{
    return true;
}

QString PhaseSimpleSwapGaming::information() const
{
    QSize xyCount = m_board->boardInfo()->xyCount();

    return QStringLiteral("%1 x %2, Swap Position [%3, %4]")
            .arg(xyCount.width()).arg(xyCount.height())
            .arg(m_swapTargetPos.x() + 1).arg(m_swapTargetPos.y() + 1);
}

} // Swap
