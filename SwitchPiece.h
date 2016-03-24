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
#ifndef SWITCHPIECE_H
#define SWITCHPIECE_H

#include "IPiece.h"
#include <QColor>

class SwitchPiece : public IPiece
{
public:
    SwitchPiece();
    ~SwitchPiece() = default;

    void open();
    void close();
    bool isOpen() const;

    void draw(QPainter &painter, const QPoint &pos) override;
    void draw(QPainter &painter, const QPoint &pos, const QSize &targetSize) override;

    void setSize(const QSize &pieceSize) override;
    void setOpenPieceOpacity(double opacity) override;

protected:
    virtual void createClosedPixmap(const QSize &pieceSize);
    virtual void drawOpenPiece(QPainter &painter, const QPoint &pos);
    virtual void drawOpenPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize);
    virtual void drawClosedPiece(QPainter &painter, const QPoint &pos);
    virtual void drawClosedPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize);
    virtual void drawCrossLine(QPainter &painter, const QPoint &pos = QPoint(0, 0));
    virtual void drawCrossLine(QPainter &painter, const QPoint &pos, const QSize &targetSize);

    const QColor foreground = QColor(224, 224, 224);
    const QColor lightLine  = QColor(192, 192, 192);
    const QColor darkLine   = QColor( 96,  96,  96);

    bool isPieceOpen;
    QPixmap closedPixmap;
    double opacity;
};

#endif // SWITCHPIECE_H
