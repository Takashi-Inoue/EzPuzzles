#ifndef POSITION_H
#define POSITION_H

#include <QPoint>

class Position
{
public:
    Position(const QPoint &defaultPosition);
    virtual ~Position() = default;

    void setPos(const QPoint &);

    QPoint defaultPos() const;
    QPoint currentPos() const;
    bool isCorrect() const;

protected:
    const QPoint defaultPosition;
    QPoint currentPosition;
};

#endif // POSITION_H
