#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QKeyEvent>

#include "piece.h"
#include "position.h"

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QObject *parent = nullptr);
    ~Game();
    void updatePosition();

public slots:
    void expiredTime(bool white);
    void keyPress(int key);
    void mousePress(int row, int col);
    void resetGame();

signals:
    void highlightSquare(int pos[2]);
//    void highlightSquare(int row, int col);
    void setPiece(int row, int col, Piece piece);

private:
    bool canMove();
    void clearSelectedSquare();
    void copyBoard(const Piece * original, Piece *copy);
    bool emptySpace(int row, int col) const;
    bool emptySpace(int pos[2]) const;
    bool equalArrays(int a[12], int b[12], int len) const;
    bool fiftyMoves() const;
    bool inCheck(bool white);
    bool inCheck(bool white, int from[2], int to[2]);
    int indexAdjustment(int rowOrColIndex) const;
    bool insufficientMaterial() const;
    bool isCheckMate();
    bool isDraw();
    bool isRepeat();
    bool isSelectable(int row, int col) const;
    bool isStalemate();
    bool makeCastleMove(int from[2], int to[2]);
    void makeMove(int from[2], int to[2]);
    bool makePassantMove(int from[2], int to[2]);
    bool makePromotionMove(int from[2], int to[2], bool autoQueen);
    void makeStandardMove(int from[2], int to[2]);
    bool opponentPiece(int row, int col, bool white) const;
    bool opponentPiece(int pos[2], bool white) const;
    Piece pieceAt(int row, int col) const;
    Piece pieceAt(int pos[2]) const;
    Position savePosition();
    void setSelectedSquare(int row, int col);
    void setStartingPosition();
    void updateCastle(int from[2], int to[2]);
    void updateFiftyMoves(Piece fromPiece, Piece toPiece);
    void updatePassant(int from[2], int to[2], Piece fromPiece);
    bool validBishopMove(int from[2], int to[2]) const;
    bool validKingMove(int from[2], int to[2]);
    bool validKnightMove(int from[2], int to[2]) const;
    bool validMove(int from[2], int to[2], bool white, bool allowChecks = false);
    bool validPawnMove(int from[2], int to[2]) const;
    bool validQueenMove(int from[2], int to[2]) const;
    bool validRookMove(int from[2], int to[2]) const;

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
    std::vector<Position> gameHistory_;
    std::vector<Position> repeatPositions_;
    bool whiteTurn_;
    int selected_[2];
    int shownMoveNumber_;
    int trueMoveNumber_;
    bool whiteKingsideCastle_;
    bool whiteQueensideCastle_;
    bool blackKingsideCastle_;
    bool blackQueensideCastle_;
    int whitePassantPawn_;
    int blackPassantPawn_;
    int movesNoProgess_;
};

#endif // GAME_H
