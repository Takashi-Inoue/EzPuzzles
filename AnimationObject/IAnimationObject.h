#ifndef IANIMATIONOBJECT_H
#define IANIMATIONOBJECT_H

class IAnimationObject
{
public:
    IAnimationObject() = default;
    virtual ~IAnimationObject() = default;

    virtual void onTickFrame() = 0;
    virtual void skipAnimation() = 0;
    virtual bool isLoopAnimation() = 0;
    virtual bool isFinishedAnimation() = 0;
};

#endif // IANIMATIONOBJECT_H
