#ifndef SLIDEBLANKPIECE_H
#define SLIDEBLANKPIECE_H

#include "IPuzzlePiece.h"
#include "BoardInformation.h"
#include "Position.h"
#include "AnimationObject/Animation/AbstractAnimation.h"

namespace Fifteen {

class SlideBlankPiece : public IPuzzlePiece
{
public:
    SlideBlankPiece(BoardInfoPointer boardInfo, const QPoint &defaultPos, const QBrush &brush, int animationFrames);

    // IPuzzlePiece
    void draw(QPainter &painter) override;
    void setPos(const QPoint &pos) override;
    void setPosWithoutAnimation(const QPoint &pos) override;
    void setAnimation(AnimationPointer) override {}
    void setEffect(EffectPointer) override;
    const AnimationPointer &animation() const override;
    const EffectPointer &effect() const override;

    const Position &pos() const override;

    // IAnimationObject
    void onTickFrame() override;
    void skipAnimation() override;
    bool isLoopAnimation() override;
    bool isFinishedAnimation() override;

private:
    BoardInfoPointer boardInfo;
    Position position;

    QBrush brush;

    QRectF oldRect;
    QRectF newRect;

    AnimationPointer oldAnimation;
    AnimationPointer newAnimation;

    EffectPointer effectObj;
};

} // Fifteen

#endif // SLIDEBLANKPIECE_H
