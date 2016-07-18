#ifndef BOARDINFORMATION_H
#define BOARDINFORMATION_H

#include <QRectF>
#include <memory>

class BoardInformation
{
public:
    BoardInformation(const QSize &xyCount, const QSize &pixelSize);

    QRectF rectFromPiecePos(const QPoint &piecePos) const;
    QPoint piecePosFromPixelPos(const QPoint &pixelPos) const;

    int xCount() const;
    int yCount() const;

private:
    QSize xyCount;
    QSize pixelSize;
};

typedef std::shared_ptr<BoardInformation> BoardInfoPointer;

#endif // BOARDINFORMATION_H
