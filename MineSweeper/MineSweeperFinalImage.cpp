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
    , m_holeImg(QStringLiteral(":/img/hole"))
{
    Q_CHECK_PTR(mineField);
    Q_CHECK_PTR(boardInfo);
}

void MineSweeperFinalImage::draw(QPainter &painter)
{
    painter.save();

    painter.setOpacity(m_mineField->openedRate());

    QSize destSize = m_pixmap.size().scaled(painter.viewport().size(), Qt::KeepAspectRatio);
    QPoint tl((painter.viewport().width() - destSize.width()) / 2
            , (painter.viewport().height() - destSize.height()) / 2);

    painter.drawPixmap(QRect(tl, destSize), m_pixmap, m_pixmap.rect());
    painter.setClipRect(QRect(tl, destSize));

    painter.setOpacity(1);

    double scale = destSize.width() / static_cast<double>(m_pixmap.width());

    for (const auto &center : explodedCenters()) {
        auto itr = std::find_if(m_matrixPairs.begin(), m_matrixPairs.end()
                              , [&](const TransformPair matrixPair)
        {
            return matrixPair.first == center.toPoint();
        });

        if (itr == m_matrixPairs.end()) {
            itr = m_matrixPairs.insert(m_matrixPairs.end()
                                     , TransformPair(center.toPoint(), createTransform()));
        }

        QPixmap pixmap = m_holeImg.transformed(itr->second);

        QSizeF holeImgDestSize = pixmap.size() * scale;

        double divW = holeImgDestSize.width()  / 2;
        double divH = holeImgDestSize.height() / 2;

        QPointF holeTL(center * scale - QPointF(divW, divH) + tl);

        painter.drawPixmap(QRectF(holeTL, holeImgDestSize), pixmap, pixmap.rect());
    }

    painter.restore();
}

QList<QPointF> MineSweeperFinalImage::explodedCenters() const
{
    QList<QPointF> centers;

    for (const auto &piecePos : m_mineField->explodedPositions()) {
        const auto &rect = m_boardInfo->rectFromPiecePos(piecePos - QPoint(1, 1));

        centers << rect.center();
    }

    return centers;
}

int MineSweeperFinalImage::piecePixelSize() const
{
    return m_pixmap.width() / m_boardInfo->countXY().width();
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
