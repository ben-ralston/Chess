#include "position.h"

#include "chess/piece.h"

Position::Position(const Position &a) :
    whiteTurn(a.whiteTurn),
    kingsideCastle(a.kingsideCastle),
    queensideCastle(a.queensideCastle),
    enPassantColumn(a.enPassantColumn)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = a.board[row][col];
        }
    }
}

Position::Position(const Piece (&newBoard)[8][8], bool newWhiteTurn, bool newKingsideCastle, bool newQueensideCastle, int newEnPassantColumn) :
    whiteTurn(newWhiteTurn),
    kingsideCastle(newKingsideCastle),
    queensideCastle(newQueensideCastle),
    enPassantColumn(newEnPassantColumn)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = newBoard[row][col];
        }
    }
}

bool Position::operator==(const Position& a) const
{
    if (whiteTurn != a.whiteTurn)
        return false;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board[r][c] != a.board[r][c])
                return false;
        }
    }

    return enPassantColumn == a.enPassantColumn &&
           kingsideCastle == a.kingsideCastle &&
           queensideCastle == a.queensideCastle;
}
