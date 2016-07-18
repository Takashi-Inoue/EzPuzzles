#include "BoardInformation.h"

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

    QPointF tl((x       * pixelSize.width()) / xCount(), (y       * pixelSize.height()) / yCount());
    QPointF br(((x + 1) * pixelSize.width()) / xCount(), ((y + 1) * pixelSize.height()) / yCount());

    return QRectF(tl, br);
}

QPoint BoardInformation::piecePosFromPixelPos(const QPoint &pixelPos) const
{
    return QPoint((pixelPos.x() * xCount()) / pixelSize.width(),
                  (pixelPos.y() * yCount()) / pixelSize.height());
}

int BoardInformation::xCount() const
{
    return xyCount.width();
}

int BoardInformation::yCount() const
{
    return xyCount.width();
}
