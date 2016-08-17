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
#include "GameCoreMineSweeper.h"

namespace MineSweeper {

GameCoreMineSweeper::GameCoreMineSweeper(std::shared_ptr<GameDataMineSweeper> gameData) :
    GameCore(gameData),
    gameDataMineSweeper(gameData),
    holeImg(":/img/hole"),
    mt(std::random_device()())
{
}

void GameCoreMineSweeper::click(const QSize &fieldSize, const QPoint &cursorPos)
{
    const auto &piecePos = piecePosFromCursorPos(fieldSize, cursorPos);

    phase->click(piecePos + QPoint(1, 1));
}

void GameCoreMineSweeper::drawFinalImage(QPainter &dest) const
{
    dest.save();

    dest.setOpacity(gameDataMineSweeper->openedRate());

    auto &finalImage = gameData->finalImage();

    QSize destSize = finalImage.size().scaled(dest.viewport().size(), Qt::KeepAspectRatio);
    QPoint tl((dest.viewport().width() - destSize.width()) / 2, (dest.viewport().height() - destSize.height()) / 2);

    dest.drawPixmap(QRect(tl, destSize), finalImage, finalImage.rect());
    dest.setClipRect(QRect(tl, destSize));

    dest.setOpacity(1);

    double scale = destSize.width() / static_cast<double>(finalImage.width());

    for (const auto &center : gameDataMineSweeper->explodedCenters()) {
        auto itr = std::find_if(matrixPairs.begin(), matrixPairs.end(), [&](const QPair<QPoint, QMatrix> matrixPair) {
            return matrixPair.first == center.toPoint();
        });

        QMatrix matrix = (itr != matrixPairs.end()) ? itr->second : QMatrix();

        if (matrix.isIdentity()) {
            matrix.rotate(mt() % 360);
            matrix.scale((mt() % 5 + 6) / 10.0, (mt() % 5 + 6) / 10.0);

            matrixPairs << QPair<QPoint, QMatrix>(center.toPoint(), matrix);
        }

        QPixmap pixmap = holeImg.transformed(matrix);

        QSizeF holeImgDestSize = pixmap.size() * scale;

        double divW = holeImgDestSize.width()  / 2;
        double divH = holeImgDestSize.height() / 2;

        QPointF holeTL(center * scale - QPointF(divW, divH) + tl);

        dest.drawPixmap(QRectF(holeTL, holeImgDestSize), pixmap, pixmap.rect());
    }

    dest.restore();
}

} // MineSweeper
