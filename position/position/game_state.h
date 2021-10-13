#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

#include <QString>

#include "chess/piece.h"
#include "position.h"
#include "move.h"


class GameState
{
public:
    GameState();

    enum VictoryType { NA, WhiteCheckmate, BlackCheckmate, DrawRepetition, DrawFifty, DrawMaterial, DrawStalemate };

    void reset();
    std::vector<Move> getLegalMoves() const;
    std::vector<Move> getPromotionMoves(const std::vector<Move> &legalMoves) const;
    bool isSelectable(int row, int col) const;
    QString makeMovePublic(int from[2], int to[2], Piece promotionPiece);
    Position savePosition() const;
    VictoryType getOutcome(const std::vector<Position> &gameHistory) const;

private:
    QString algebraicNotation(int from[2], int to[2], Piece fromPiece,
                              Piece toPiece, Piece promotionPiece) const;
    bool canMove() const;
    bool canMove(bool whiteTurn, const Piece (&board)[8][8]) const;
    char colToFile(int col) const;
    void copyBoard(const Piece (&original)[8][8], Piece (&copy)[8][8]) const;
    bool emptySpace(int row, int col, const Piece (&board)[8][8]) const;
    bool emptySpace(int pos[2], const Piece (&board)[8][8]) const;
    bool equalArrays(int a[], int b[], int len) const;
    bool fiftyMoves() const;
    bool inCheck(bool whiteTurn, const Piece (&board)[8][8]) const;
    bool inCheck(bool whiteTurn, int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool insufficientMaterial() const;
    bool isPromotionMove(int from[2], int to[2]) const;
    bool isRepeat(const std::vector<Position> &gameHistory) const;
    bool legalBishopMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalKingMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalKnightMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks = false) const;
    bool legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks, const Piece (&board)[8][8]) const;
    bool legalPawnMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalQueenMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool legalRookMove(int from[2], int to[2], const Piece (&board)[8][8]) const;
    bool makeCastleMove(int from[2], int to[2]);
    bool makeCastleMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    void makeMove(int from[2], int to[2]);
    void makeMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    bool makePassantMove(int from[2], int to[2]);
    bool makePassantMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    void makePromotionMove(int from[2], int to[2], Piece promotionPiece);
    void makeStandardMove(int from[2], int to[2]);
    void makeStandardMove(int from[2], int to[2], Piece (&board)[8][8]) const;
    QString notationAmbiguity(int from[2], int to[2]) const;
    bool opponentPiece(int pos[2], bool whiteTurn, const Piece (&board)[8][8]) const;
    Piece pieceAt(int row, int col, const Piece (&board)[8][8]) const;
    Piece pieceAt(int pos[2], const Piece (&board)[8][8]) const;
    char rowToRank(int row) const;
    void updateCastle(int from[2], int to[2]);
    void updateFiftyMoves(Piece fromPiece, Piece toPiece);
    void updatePassant(int from[2], int to[2], Piece fromPiece);

    Piece board_[8][8];
    bool whiteTurn_;
    bool whiteKingsideCastle_;
    bool whiteQueensideCastle_;
    bool blackKingsideCastle_;
    bool blackQueensideCastle_;
    int whitePassantColumn_;
    int blackPassantColumn_;
    int movesNoProgess_;

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
};

#endif // GAMESTATE_H
