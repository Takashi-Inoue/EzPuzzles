#ifndef FRAMETIMER_H
#define FRAMETIMER_H

#include "ThreadOperation.h"

class FrameTimer : public ThreadOperation
{
    Q_OBJECT
public:
    FrameTimer(double fps = 60.0);
    ~FrameTimer() = default;

    void stop() override;

signals:
    void tick(QMutex *mutexOperation, QWaitCondition *waitOperation);

protected:
    QString className() const override;
    void execImpl() override;

private:
    QMutex mutexOperation;
    QWaitCondition waitOperation;

    double msecsPerFrame;
};

#endif // FRAMETIMER_H
