#include "move.h"

Move::Move(QObject *parent) : QObject(parent)
{
    direction=Straight;

}
DIRECTION Move::getDirection() const
{
    return direction;
}


