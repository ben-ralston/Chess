#ifndef GAME_H
#define GAME_H

#include <array>
#include <iostream>
#include <vector>
#include "piece.h"
#include "position.h"

class Game
{
public:
    Game();
    Piece * getPosition();
    bool isSelectable(int row, int col);
    void tryMove(int from[2], int to[2]);
    void resetGame();

private:
    Piece position[8][8];
    Piece flippedPosition[8][8];
    std::vector<Position> gameHistory;
    std::vector<Position> repeatPositions;
    bool whiteTurn;
    bool whiteKingsideCastle;
    bool whiteQueensideCastle;
    bool blackKingsideCastle;
    bool blackQueensideCastle;
    int whitePassantPawn;
    int blackPassantPawn;
    int movesNoProgess;
    void setStartingPosition();
    Position savePosition();
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
    bool fiftyMoves();
    bool isRepeat();
    bool equalArrays(int a[12], int b[12]);
    bool canMove();
    void updateCastle(int from[2], int to[2]);
    void updatePassant(int from[2], int to[2], Piece fromPiece);
    void updateFiftyMoves(Piece fromPiece, Piece toPiece);

};

#endif // GAME_H
