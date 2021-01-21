/*
 * Copyright 2021 Takashi Inoue
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

#include "SwapEndingBoard.h"

#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"

#include "AnimationFactory.h"
#include "BoardInformation.h"
#include "Fifteen/FifteenFactory.h"
#include "Fifteen/IPuzzlePiece.h"

#include <QPainter>

namespace Swap {

EndingBoard::EndingBoard(BoardInfoPointer boardInfo, const QPixmap &sourceImage)
    : m_pieces(Fifteen::Factory().createPieces(boardInfo, sourceImage))
    , m_sourceImage(sourceImage)
{
    for (FifteenPiecePointer &piece : m_pieces) {
        auto graduallyFrame = AnimationFactory::graduallyBlinkFrame(
                                  2, "#FFFF8040", "#80FFFF40"
                                   , "#80FFFF40", "#FFFF8040", 120);

        piece->setEffect(graduallyFrame);
    }
}

void EndingBoard::draw(QPainter &painter)
{
    for (FifteenPiecePointer &piece : m_pieces)
        piece->draw(painter);

    QPolygon polygon = {QPoint(0, 0)};

    const qreal elapsed = qreal(m_elapsedTimer.elapsed());
    const QRect imageRect = m_sourceImage.rect();

    if (elapsed < m_halfMSec) {
        qreal percentage = elapsed / m_halfMSec;

        polygon << QPoint(int(imageRect.width() * percentage), 0)
                << QPoint(0, int(imageRect.height() * percentage));
    } else {
        qreal percentage = (elapsed - m_halfMSec) / m_halfMSec;

        polygon << m_sourceImage.rect().topRight()
                << QPoint(imageRect.right(), int(imageRect.height() * percentage))
                << QPoint(int(imageRect.width() * percentage), imageRect.height())
                << m_sourceImage.rect().bottomLeft();
    }

    painter.setClipRegion(polygon);
    painter.drawPixmap(QPoint(0, 0), m_sourceImage);
}

bool EndingBoard::onTickFrame()
{
    if (!m_elapsedTimer.isValid())
        m_elapsedTimer.start();

    for (FifteenPiecePointer &piece : m_pieces)
        piece->onTickFrame();

    return m_elapsedTimer.elapsed() < m_halfMSec * 2;
}

} // namespace Swap
