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
#ifndef BLOCKPIECE_H
#define BLOCKPIECE_H

#include "IPiece.h"
#include <QColor>
#include <QMap>

class BlockPiece : public IPiece
{
public:
    BlockPiece(const QSize &size = QSize(0, 0), QColor foreground = QColor(224, 224, 224), QColor lightLine = QColor(192, 192, 192), QColor darkLine = QColor(96, 96, 96));
    ~BlockPiece() = default;

    // IPiece
    void draw(QPainter &painter, const QPointF &pos) override;
    void draw(QPainter &painter, const QPointF &pos, const QSizeF &targetSize) override;

protected:
    class Key
    {
    public:
        Key(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3);
        ~Key() = default;

        bool operator<(const Key &other) const;

    private:
        uint64_t sizeInt;
        uint64_t color1;
        uint color2;
    };

    static QMap<Key, QPixmap> pixmapMap;

    void drawPiece(QPainter &painter, const QPointF &pos, const QSizeF &targetSize);

    QColor foregroundColor;
    QColor lightLineColor;
    QColor darkLineColor;

    QPixmap pixmap;
};

#endif // BLOCKPIECE_H
