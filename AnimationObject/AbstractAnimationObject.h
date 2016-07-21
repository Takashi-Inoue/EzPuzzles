#ifndef ABSTRACTANIMATIONOBJECT_H
#define ABSTRACTANIMATIONOBJECT_H

#include "IAnimationObject.h"

class AbstractAnimationObject : public IAnimationObject
{
public:
    AbstractAnimationObject(int totalFrameCount, bool isLoop);

    void onTickFrame() override;
    void skipAnimation() override;
    bool isLoopAnimation() override;
    bool isFinishedAnimation() override;

protected:
    void resetFrame();
    double totalFrames() const;
    double nowFrame() const;

private:
    int totalFrameCount;
    int nowFrameCount;
    bool isLoop;
};

#endif // ABSTRACTANIMATIONOBJECT_H
