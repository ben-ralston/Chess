#include "move.h"



Move::Move(int newMove[4])
{
    for (int i = 0; i < 4; i++) {
        move[i] = newMove[i];
    }
}

Move::Move(int from[2], int to[2])
{
    move[0] = from[0];
    move[1] = from[1];
    move[2] = to[0];
    move[3] = to[1];
}

Move::Move(int fromRow, int fromCol, int toRow, int toCol)
{
    move[0] = fromRow;
    move[1] = fromCol;
    move[2] = toRow;
    move[3] = toCol;
}

bool Move::operator==(const Move &a) const
{
    for (int i = 0; i < 4; i++) {
        if (move[i] != a.move[i])
            return false;
    }

    return true;
}

bool Move::operator<(const Move &a) const
{
    for (int i = 0; i < 3; i++) {
        if (move[i] < a.move[i])
            return true;
    }

    return move[3] < a.move[3];
}

bool Move::operator>(const Move &a) const
{
    return !(operator==(a) || operator<(a));
}

void Move::getFromTo(int (&from)[2], int (&to)[2]) const
{
    from[0] = move[0];
    from[1] = move[1];
    to[0] = move[2];
    to[1] = move[3];
}
