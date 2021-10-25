#ifndef POSITION_H
#define POSITION_H

#include "chess/piece.h"

struct Position
{
    Position(const Position &a);
    Position(const Piece (&newBoard)[8][8], bool newWhiteTurn, bool newKingsideCastle,
             bool newQueensideCastle, int newEnPassantColumn);
    bool operator==(const Position& a) const;

    Piece board[8][8];
    bool whiteTurn;
    bool kingsideCastle;
    bool queensideCastle;
    int enPassantColumn;
};

#endif // POSITION_H
