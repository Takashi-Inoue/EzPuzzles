﻿#include "SlideBlankPiece.h"
#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Transform/TransformExpand.h"

#include <QDebug>

namespace Fifteen {

SlideBlankPiece::SlideBlankPiece(BoardInfoPointer boardInfo, const QPoint &defaultPos, const QBrush &brush, int animationFrames) :
    boardInfo(boardInfo),
    position(defaultPos),
    brush(brush),
    oldAnimation(QSharedPointer<Animation::LineMove>::create(animationFrames, false)),
    newAnimation(QSharedPointer<Animation::LineMove>::create(animationFrames, false))
{
}

bool SlideBlankPiece::onTickFrame()
{
    bool result = false;

    result |= oldAnimation->onTickFrame();
    result |= newAnimation->onTickFrame();

    if (effectObj != nullptr)
        result |= effectObj->onTickFrame();

    return result;
}

void SlideBlankPiece::skipAnimation()
{
    oldAnimation->skipAnimation();
    newAnimation->skipAnimation();
}

void SlideBlankPiece::draw(QPainter &painter)
{
    oldRect = oldAnimation->rect();

    QRectF rect = newAnimation->rect();

    if (rect.isValid())
        newRect = rect;

    if (oldRect.isValid()) {
        painter.fillRect(oldRect, brush);

        if (effectObj != nullptr)
            effectObj->draw(painter, oldRect);
    }

    if (newRect.isValid()) {
        painter.fillRect(newRect, brush);

        if (effectObj != nullptr)
            effectObj->draw(painter, newRect);
    }
}

void SlideBlankPiece::setPos(const QPoint &pos)
{
    const auto &currentPos = position.currentPos();

    if (currentPos == pos)
        return;

    if ((currentPos.x() != pos.x()) & (currentPos.y() != pos.y())) {
        qDebug() << pos;
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

    position.setPos(pos);
}

void SlideBlankPiece::setEffect(EffectPointer effect)
{
    this->effectObj = effect;
}

AnimationPointer SlideBlankPiece::animation() const
{
    return newAnimation;
}

EffectPointer SlideBlankPiece::effect() const
{
    return effectObj;
}

TransformPointer SlideBlankPiece::transform() const
{
    return nullptr;
}

const Position &SlideBlankPiece::pos() const
{
    return position;
}

} // Fifteen
