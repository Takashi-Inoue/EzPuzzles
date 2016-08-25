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

namespace MineSweeper {

MineSweeperFinalImage::MineSweeperFinalImage(QPixmap pixmap, MineFieldPointer mineField, BoardInfoPointer boardInfo) :
    FinalImage(pixmap),
    mineField(mineField),
    boardInfo(boardInfo),
    holeImg(":/img/hole"),
    mt(std::random_device()())
{
    Q_CHECK_PTR(mineField);
    Q_CHECK_PTR(boardInfo);
}

void MineSweeperFinalImage::draw(QPainter &painter)
{
    painter.save();

    painter.setOpacity(mineField->openedRate());

    QSize destSize = pixmap.size().scaled(painter.viewport().size(), Qt::KeepAspectRatio);
    QPoint tl((painter.viewport().width() - destSize.width()) / 2, (painter.viewport().height() - destSize.height()) / 2);

    painter.drawPixmap(QRect(tl, destSize), pixmap, pixmap.rect());
    painter.setClipRect(QRect(tl, destSize));

    painter.setOpacity(1);

    double scale = destSize.width() / static_cast<double>(pixmap.width());

    for (const auto &center : explodedCenters()) {
        auto itr = std::find_if(matrixPairs.begin(), matrixPairs.end(), [&](const MatrixPair matrixPair) {
            return matrixPair.first == center.toPoint();
        });

        if (itr == matrixPairs.end())
            itr = matrixPairs.insert(matrixPairs.end(), MatrixPair(center.toPoint(), createMatrix()));

        QPixmap pixmap = holeImg.transformed(itr->second);

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

    for (const auto &piecePos : mineField->explodedPositions()) {
        const auto &rect = boardInfo->rectFromPiecePos(piecePos - QPoint(1, 1));

        centers << rect.center();
    }

    return centers;
}

int MineSweeperFinalImage::piecePixelSize() const
{
    return pixmap.width() / boardInfo->boardSize().width();
}

QMatrix MineSweeperFinalImage::createMatrix()
{
    double minHoleSize = piecePixelSize() * 3;
    double scaleToPieceSize = qMax(minHoleSize / holeImg.width(), 1.0);

    double mineRatio = mineField->mineRatio();
    double scaleBase = (mineRatio * mineRatio * mineRatio * 70) + scaleToPieceSize;

    QMatrix matrix;

    matrix.rotate(mt() % 360);
    matrix.scale(scaleBase + (mt() % 5 + 6) / 10.0, scaleBase + (mt() % 5 + 6) / 10.0);

    return matrix;
}

} // MineSweeper
