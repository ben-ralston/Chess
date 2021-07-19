#ifndef POSITION_H
#define POSITION_H

#include "piece.h"

struct Position {
    Piece board[8][8];
    bool whiteTurn;
    bool kingsideCastle;
    bool queensideCastle;
    int enPassantCol;

    bool operator==(Position a) {
        if (whiteTurn != a.whiteTurn)
            return false;

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c] != a.board[r][c])
                    return false;
            }
        }

        return enPassantCol == a.enPassantCol
               && kingsideCastle == a.kingsideCastle
               && queensideCastle == a.queensideCastle;
    }
};

#endif // POSITION_H
