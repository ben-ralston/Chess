#ifndef MOVE_H
#define MOVE_H

#include "chess/piece.h"

struct Move
{
    Move();
    Move(const Move &a);
    Move(int newMove[4]);
    Move(int from[2], int to[2], Piece piece = None);
    Move(int fromRow, int fromCol, int toRow, int toCol);
    void operator=(const Move &a);
    bool operator==(const Move &a) const;
    bool operator<(const Move &a) const;
    bool operator>(const Move &a) const;
    void getFromTo(int (&from)[2], int (&to)[2]) const;

    int move[4];
    Piece promotionPiece;
};

#endif // MOVE_H
