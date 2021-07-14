#ifndef GAME_H
#define GAME_H

#include <array>
#include <iostream>
#include "piece.h"

class Game
{
public:
    Game();
    Piece * getPosition();
    bool isSelectable(int row, int col);
    void tryMove(int from[2], int to[2]);

private:
    Piece position[8][8];
    bool whiteTurn;
    bool whiteKingsideCastle;
    bool whiteQueensideCastle;
    bool blackKingsideCastle;
    bool blackQueensideCastle;
    int whitePassantPawn;
    int blackPassantPawn;
    void setStartingPosition();
    void makeMove(int from[2], int to[2]);
    void makeStandardMove(int from[2], int to[2]);
    bool makeCastleMove(int from[2], int to[2]);
    bool makePassantMove(int from[2], int to[2]);
    bool makePromotionMove(int from[2], int to[2], bool autoQueen);
    bool validMove(int from[2], int to[2]);
    bool validPawnMove(int from[2], int to[2], bool white);
    bool validKnightMove(int from[2], int to[2], bool white);
    bool validBishopMove(int from[2], int to[2], bool white);
    bool validRookMove(int from[2], int to[2], bool white);
    bool validQueenMove(int from[2], int to[2], bool white);
    bool validKingMove(int from[2], int to[2], bool white);
    bool emptySpace(int r, int c);
    bool emptySpace(int pos[2]);
    bool opponentPiece(int r, int c, bool white);
    bool opponentPiece(int pos[2], bool white);
    bool inCheck(bool white);
    bool inCheck(bool white, int from[2], int to[2]);
    bool isCheckMate();
    bool isDraw();
    bool isStalemate();
    bool insufficientMaterial();
    bool equalArrays(int a[12], int b[12]);
    bool canMove();
    void updateCastle(int from[2], int to[2]);
    void updatePassant(int from[2], int to[2], Piece fromPiece);

};

#endif // GAME_H
