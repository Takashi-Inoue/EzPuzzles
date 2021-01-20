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
    bool onTickFrame() override;
    void skipAnimation() override;

    void draw(QPainter &painter) override;
    void setPos(const QPoint &pos) override;
    void setPosWithoutAnimation(const QPoint &pos) override;
    void setAnimation(AnimationPointer) override {}
    void setTransform(TransformPointer) override {}
    void setEffect(EffectPointer) override;
    AnimationPointer animation() const override;
    EffectPointer effect() const override;
    TransformPointer transform() const override;

    const Position &pos() const override;

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
