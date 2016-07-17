#include "boardinformation.h"

BoardInformation::BoardInformation(const QSize &xyCount, const QSize &pixelSize) :
    xyCount(xyCount),
    pixelSize(pixelSize)
{
    Q_ASSERT(!xyCount.isEmpty());
    Q_ASSERT(!pixelSize.isEmpty());
}

QRectF BoardInformation::rectFromPiecePos(const QPoint &piecePos) const
{
    double x = piecePos.x();
    double y = piecePos.y();

    QPointF tl((x       * pixelSize.width()) / xyCount.width(), (y       * pixelSize.height()) / xyCount.height());
    QPointF br(((x + 1) * pixelSize.width()) / xyCount.width(), ((y + 1) * pixelSize.height()) / xyCount.height());

    return QRectF(tl, br);
}

QPoint BoardInformation::piecePosFromPixelPos(const QPoint &pixelPos) const
{
    return QPoint((pixelPos.x() * xyCount.width())  / pixelSize.width(),
                  (pixelPos.y() * xyCount.height()) / pixelSize.height());
}
