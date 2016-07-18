#include "Position.h"

Position::Position(const QPoint &defaultPosition) :
    defaultPosition(defaultPosition),
    currentPosition(defaultPosition)
{
}

void Position::setPos(const QPoint &pos)
{
    currentPosition = pos;
}

QPoint Position::defaultPos() const
{
    return defaultPosition;
}

QPoint Position::currentPos() const
{
    return currentPosition;
}

bool Position::isCorrect() const
{
    return defaultPosition == currentPosition;
}
