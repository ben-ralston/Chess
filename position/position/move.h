#ifndef MOVE_H
#define MOVE_H

struct Move
{
    Move(int newMove[4]);
    Move(int from[2], int to[2]);
    Move(int fromRow, int fromCol, int toRow, int toCol);
    bool operator==(const Move &a) const;
    bool operator<(const Move &a) const;
    bool operator>(const Move &a) const;
    void getFromTo(int (&from)[2], int (&to)[2]) const;

    int move[4];
};

#endif // MOVE_H
