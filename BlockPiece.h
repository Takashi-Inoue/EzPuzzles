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
    BlockPiece(const QSize &size = QSize(0, 0), QColor foreground = QColor(224, 224, 224), QColor inner = QColor(192, 192, 192), QColor outer = QColor(96, 96, 96));
public:
    enum Colors {gray, grayPressed, blue, red};

    static QSharedPointer<BlockPiece> create(const QSize &size, Colors color);
    static QSharedPointer<BlockPiece> create(const QSize &size, QColor foreground
                                           , QColor inner, QColor outer);

    // IPiece
    void draw(QPainter &painter, const QPointF &pos) override;
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    class Info
    {
    public:
        Info(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3);
        ~Info() = default;

        bool operator<(const Info &other) const;

    private:
        int64_t m_sizeInt;
        uint64_t m_color1;
        uint m_color2;
    };

    QPixmap createPixmap(const QSize &size, QColor foreground, QColor inner, QColor outer) const;

    QPixmap m_pixmap;
};

#endif // BLOCKPIECE_H
