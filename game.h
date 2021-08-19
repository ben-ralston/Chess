#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include "piece.h"
#include "position.h"

class Game
{
public:
    Game();
    ~Game();
    void resetGame();
    int tryMove(int from[2], int to[2]);
    Position getPosition(int moveNum = -1) const;
    bool isSelectable(int row, int col) const;

private:
    bool canMove();
    bool emptySpace(int r, int c);
    bool emptySpace(int pos[2]);
    bool equalArrays(int a[12], int b[12]);
    bool fiftyMoves();
    bool inCheck(bool white);
    bool inCheck(bool white, int from[2], int to[2]);
    bool insufficientMaterial();
    bool isCheckMate();
    bool isDraw();
    bool isRepeat();
    bool isStalemate();
    bool makeCastleMove(int from[2], int to[2]);
    void makeMove(int from[2], int to[2]);
    bool makePassantMove(int from[2], int to[2]);
    bool makePromotionMove(int from[2], int to[2], bool autoQueen);
    void makeStandardMove(int from[2], int to[2]);
    bool opponentPiece(int r, int c, bool white);
    bool opponentPiece(int pos[2], bool white);
    Position savePosition();
    void setStartingPosition();
    void updateCastle(int from[2], int to[2]);
    void updateFiftyMoves(Piece fromPiece, Piece toPiece);
    void updatePassant(int from[2], int to[2], Piece fromPiece);
    bool validBishopMove(int from[2], int to[2], bool white);
    bool validKingMove(int from[2], int to[2], bool white);
    bool validKnightMove(int from[2], int to[2], bool white);
    bool validMove(int from[2], int to[2]);
    bool validPawnMove(int from[2], int to[2], bool white);
    bool validQueenMove(int from[2], int to[2], bool white);
    bool validRookMove(int from[2], int to[2], bool white);

    const Piece startingPosition_[8][8] = {
        { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook },
        { BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn },
        { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook },
    };
    Piece position_[8][8];
    Piece flippedPosition_[8][8];
    std::vector<Position> gameHistory_;
    std::vector<Position> repeatPositions_;
    bool whiteTurn_;
    bool whiteKingsideCastle_;
    bool whiteQueensideCastle_;
    bool blackKingsideCastle_;
    bool blackQueensideCastle_;
    int whitePassantPawn_;
    int blackPassantPawn_;
    int movesNoProgess_;
};

#endif // GAME_H
