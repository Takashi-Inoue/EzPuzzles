#ifndef BOARDINFORMATION_H
#define BOARDINFORMATION_H

#include <QRectF>

class BoardInformation
{
public:
    BoardInformation(const QSize &xyCount, const QSize &pixelSize);

    QRectF rectFromPiecePos(const QPoint &piecePos) const;
    QPoint piecePosFromPixelPos(const QPoint &pixelPos) const;

private:
    QSize xyCount;
    QSize pixelSize;
};

#endif // BOARDINFORMATION_H
