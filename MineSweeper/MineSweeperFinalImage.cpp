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
#include "MineSweeperFinalImage.h"

#include <QRandomGenerator>

namespace MineSweeper {

MineSweeperFinalImage::MineSweeperFinalImage(QPixmap pixmap, MineFieldPointer mineField
                                           , BoardInfoPointer boardInfo)
    : FinalImage(pixmap)
    , m_mineField(mineField)
    , m_boardInfo(boardInfo)
    , m_holeImg(QStringLiteral(":/images/hole"))
{
    Q_CHECK_PTR(mineField);
    Q_CHECK_PTR(boardInfo);
}

void MineSweeperFinalImage::draw(QPainter &painter)
{
    painter.save();

    painter.setOpacity(m_mineField->openedRate());

    QRect destRect = drawFinalImage(painter);

    painter.setOpacity(1);
    painter.setClipRect(destRect);

    double scale = destRect.width() / double(m_pixmap.width());

    for (const QPointF &center : explodedCenters()) {
        auto itr = std::find_if(m_matrixPairs.begin(), m_matrixPairs.end()
                              , [&](const TransformPair &matrixPair)
        {
            return matrixPair.first == center.toPoint();
        });

        if (itr == m_matrixPairs.end())
            itr = m_matrixPairs.insert(itr, TransformPair(center.toPoint(), createTransform()));

        QPixmap holeImg = m_holeImg.transformed(itr->second);

        QSizeF holeImgDestSize = holeImg.size() * scale;

        double divW = holeImgDestSize.width()  / 2;
        double divH = holeImgDestSize.height() / 2;

        QPointF holeTL(center * scale - QPointF(divW, divH) + destRect.topLeft());

        painter.drawPixmap(QRectF(holeTL, holeImgDestSize), holeImg, holeImg.rect());
    }

    painter.restore();
}

QList<QPointF> MineSweeperFinalImage::explodedCenters() const
{
    QList<QPointF> centers;

    for (const QPoint &piecePos : m_mineField->explodedPositions()) {
        const QRectF &rect = m_boardInfo->rectFromPiecePos(piecePos - QPoint(1, 1));

        centers << rect.center();
    }

    return centers;
}

int MineSweeperFinalImage::piecePixelSize() const
{
    return m_pixmap.width() / m_boardInfo->xyCount().width();
}

QTransform MineSweeperFinalImage::createTransform()
{
    double minHoleSize = piecePixelSize() * 3;
    double scaleToPieceSize = qMax(minHoleSize / m_holeImg.width(), 1.0);

    double mineRatio = m_mineField->mineRatio();
    double scaleBase = (mineRatio * mineRatio * mineRatio * 70) + scaleToPieceSize;

    QTransform transform;
    QRandomGenerator *randomGenerator = QRandomGenerator::global();

    transform.rotate(randomGenerator->bounded(360));
    transform.scale(scaleBase + (randomGenerator->bounded(5) + 6) / 10.0
                  , scaleBase + (randomGenerator->bounded(5) + 6) / 10.0);

    return transform;
}

} // MineSweeper
