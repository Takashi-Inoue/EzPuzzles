#include "SlideBlankPiece.h"
#include "AnimationObject/Animation/AnimationLineMove.h"

namespace Fifteen {

SlideBlankPiece::SlideBlankPiece(BoardInfoPointer boardInfo, const QPoint &defaultPos, const QBrush &brush, int animationFrames) :
    boardInfo(boardInfo),
    position(defaultPos),
    brush(brush),
    oldAnimation(std::make_shared<Animation::LineMove>(animationFrames, false)),
    newAnimation(std::make_shared<Animation::LineMove>(animationFrames, false))
{
}

void SlideBlankPiece::draw(QPainter &painter)
{
    oldRect = oldAnimation->rect();
    newRect = newAnimation->rect();

    if (oldRect.isValid()) {
        painter.fillRect(oldRect, brush);

        if (effect != nullptr)
            effect->draw(painter, oldRect);
    }

    if (newRect.isValid()) {
        painter.fillRect(newRect, brush);

        if (effect != nullptr)
            effect->draw(painter, newRect);
    }
}

void SlideBlankPiece::setPos(const QPoint &pos)
{
    const auto &currentPos = position.currentPos();

    if (currentPos == pos)
        return;

    if ((currentPos.x() != pos.x()) & (currentPos.y() != pos.y())) {
        Q_ASSERT_X(false, "SlideBlankPiece::setPos()", "wrong new pos");
        return;
    }

    QRectF newEndRect = boardInfo->rectFromPiecePos(pos);
    QRectF newStartRect = newEndRect;
    QRectF oldStartRect = boardInfo->rectFromPiecePos(position.currentPos());
    QRectF oldEndRect = oldStartRect;

    if (currentPos.x() == pos.x()) {
        if (currentPos.y() < pos.y()) {
            newStartRect.setTop(newStartRect.bottom());
            oldEndRect.setBottom(oldEndRect.top());
        } else {
            newStartRect.setBottom(newStartRect.top());
            oldEndRect.setTop(oldEndRect.bottom());
        }
    } else {
        if (currentPos.x() < pos.x()) {
            newStartRect.setLeft(newStartRect.right());
            oldEndRect.setRight(oldEndRect.left());
        } else {
            newStartRect.setRight(newStartRect.left());
            oldEndRect.setLeft(oldEndRect.right());
        }
    }

    oldAnimation->start(oldStartRect, oldEndRect);
    newAnimation->start(newStartRect, newEndRect);

    position.setPos(pos);
}

void SlideBlankPiece::setPosWithoutAnimation(const QPoint &pos)
{
    oldRect = QRectF();
    newRect = boardInfo->rectFromPiecePos(pos);
}

void SlideBlankPiece::setEffect(EffectPointer effect)
{
    this->effect = effect;
}

const Position &SlideBlankPiece::pos() const
{
    return position;
}

void SlideBlankPiece::onTickFrame()
{
    oldAnimation->onTickFrame();
    newAnimation->onTickFrame();

    if (effect != nullptr)
        effect->onTickFrame();
}

void SlideBlankPiece::skipAnimation()
{
    oldAnimation->skipAnimation();
    newAnimation->skipAnimation();
}

bool SlideBlankPiece::isLoopAnimation()
{
    return false;
}

bool SlideBlankPiece::isFinishedAnimation()
{
    return false;
}

} // Fifteen
