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
#include "BlockPiece.h"

#include <QPainter>
#include <QDebug>

BlockPiece::BlockPiece(const QSize &size, QColor foreground, QColor inner, QColor outer)
    : m_pixmap(createPixmap(size, foreground, inner, outer))
{
    Q_ASSERT(!size.isEmpty());
}

QSharedPointer<BlockPiece> BlockPiece::create(const QSize &size, BlockPiece::Colors color)
{
    static const QHash<Colors, QList<QColor>> colorHash = {
        {gray,        {QColor("#E0E0E0"), QColor("#C0C0C0"), QColor("#606060")}},
        {grayPressed, {QColor("#C0C0C0"), QColor("#D0D0D0"), QColor("#606060")}},
        {red,         {QColor("#E08080"), QColor("#C06060"), QColor("#600000")}},
        {blue,        {QColor("#A0A0FF"), QColor("#8080E0"), QColor("#202080")}},
    };

    const QList<QColor> &colors = colorHash[color];

    return create(size, colors[0], colors[1], colors[2]);
}

QSharedPointer<BlockPiece> BlockPiece::create(const QSize &size, QColor foreground
                                            , QColor inner, QColor outer)
{
    static QMap<Info, QSharedPointer<BlockPiece>> pieceMap;

    Info info(size, foreground.rgba(), inner.rgba(), outer.rgba());

    QSharedPointer<BlockPiece> &piece = pieceMap[info];

    if (piece == nullptr) {
        qDebug() << QStringLiteral("create BlockPiece: size=%1, fore=%2, inner=%3, outer=%4")
                    .arg(size.width())
                    .arg(foreground.name(QColor::HexArgb), inner.name(QColor::HexArgb), outer.name(QColor::HexArgb));

        auto newPiece = new BlockPiece(size, foreground, inner, outer);
        piece = QSharedPointer<BlockPiece>(newPiece);
    }

    return piece;
}

void BlockPiece::draw(QPainter &painter, const QPointF &pos)
{
    draw(painter, QRectF(pos, m_pixmap.size()));
}

void BlockPiece::draw(QPainter &painter, const QRectF &rect)
{
    painter.drawPixmap(rect, m_pixmap, m_pixmap.rect());
}

QPixmap BlockPiece::createPixmap(const QSize &size, QColor foreground, QColor inner, QColor outer) const
{
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    QPoint pos(0, 0);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    painter.setOpacity(1.0);

    painter.fillRect(QRect(pos, size - QSize(1, 1)), foreground);

    painter.setPen(outer);
    painter.drawRect(QRect(pos, size - QSize(1, 1)));

    painter.setPen(inner);
    painter.drawRect(QRect(pos + QPoint(1, 1), size - QSize(3, 3)));

    return pixmap;
}

// class BlockPiece::Info
BlockPiece::Info::Info(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3)
    : m_sizeInt(size.width())
    , m_color1(rgba1)
    , m_color2(rgba3)
{
    Q_ASSERT(!size.isEmpty());

    m_sizeInt <<= 32;
    m_sizeInt |= size.height();

    m_color1 <<= 32;
    m_color1 |= rgba2;
}

bool BlockPiece::Info::operator<(const BlockPiece::Info &other) const
{
    if (m_sizeInt != other.m_sizeInt)
        return m_sizeInt < other.m_sizeInt;

    if (m_color1 != other.m_color1)
        return m_color1 < other.m_color1;

    return m_color2 < other.m_color2;
}
