#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

#include <QString>

#include "chess/piece.h"
#include "position.h"
#include "move.h"

class GameState
{
public:
    enum VictoryType { NA, WhiteCheckmate, BlackCheckmate, DrawRepetition, DrawFifty, DrawMaterial, DrawStalemate };

    GameState();
//    GameState(const GameState &a);
    void getLegalMoves(std::vector<Move> &output) const;
    void getPromotionMoves(const std::vector<Move> &legalMoves, std::vector<Move> &output) const;
    Position savePosition() const;
    VictoryType getOutcome(const std::vector<Position> &gameHistory) const;
    void reset();

protected:
    bool canMove(bool whiteTurn, const Piece (&board)[8][8]) const;
    void copyBoard(const Piece (&original)[8][8], Piece (&copy)[8][8]) const;
    bool equalArrays(int a[], int b[], int len) const;
    bool inCheck(bool whiteTurn, int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks = false) const;
    void makeMove(int from[2], int to[2]);
    void makeMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    void makeMoveAndUpdate(int from[2], int to[2], Piece promotionPiece);
    void makePromotionMove(int from[2], int to[2], Piece promotionPiece);
    bool opponentPiece(int pos[2], bool whiteTurn, const Piece (&board)[8][8]) const;
    Piece pieceAt(int pos[2], const Piece (&board)[8][8]) const;
    void updateCastle(int from[2], int to[2]);
    void updateFiftyMoves(Piece fromPiece, Piece toPiece);
    void updatePassant(int from[2], int to[2], Piece fromPiece);

    Piece board_[8][8];
    bool whiteTurn_;

private:
    static const Piece startingPosition_[8][8];

    bool emptySpace(int row, int col, const Piece (&board)[8][8]) const;
    bool emptySpace(int pos[2], const Piece (&board)[8][8]) const;
    bool fiftyMoves() const;
    bool inCheck(bool whiteTurn, const Piece (&board)[8][8]) const;
    bool insufficientMaterial() const;
    bool isPromotionMove(int from[2], int to[2]) const;
    bool isRepeat(const std::vector<Position> &gameHistory) const;
    bool legalBishopMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalKingMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalKnightMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks, const Piece (&board)[8][8]) const;
    bool legalPawnMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalQueenMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalRookMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool makeCastleMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    bool makePassantMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    void makeStandardMove(int from[2], int to[2], Piece (&board)[8][8]) const;

    bool whiteKingsideCastle_;
    bool whiteQueensideCastle_;
    bool blackKingsideCastle_;
    bool blackQueensideCastle_;
    int whitePassantColumn_;
    int blackPassantColumn_;
    int movesNoProgess_;
};

#endif // GAME_STATE_H
