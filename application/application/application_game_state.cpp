#include "application_game_state.h"

#include <QString>

#include "rules/game_state.h"
#include "chess/piece.h"

ApplicationGameState::ApplicationGameState() : GameState()
{
}

bool ApplicationGameState::isSelectable(int row, int col) const
{
    int pos[2] = { row, col };
    return opponentPiece(pos, !whiteTurn_, board_);
}

QString ApplicationGameState::move(int from[2], int to[2], Piece promotionPiece)
{
    QString notationString = algebraicNotation(from, to, promotionPiece);
    makeMoveAndUpdate(from, to, promotionPiece);

    return notationString;
}

QString ApplicationGameState::algebraicNotation(int from[2], int to[2], Piece promotionPiece) const
{
    Piece fromPiece = pieceAt(from, board_);
    Piece toPiece = pieceAt(to, board_);

    if (fromPiece == WhiteKing || fromPiece == BlackKing) {
        if (to[1] - from[1] == 2)
            return QString("O-O");
        if (to[1] - from[1] == -2)
            return QString("O-O-O");
    }

    bool takes;
    if (fromPiece == WhitePawn || fromPiece == BlackPawn)
        takes = from[1] != to[1];
    else
        takes = toPiece != None;

    QString move;
    if (fromPiece == WhitePawn || fromPiece == BlackPawn) {
        if (takes)
            move = colToFile(from[1]);
    } else if (fromPiece == WhiteKnight || fromPiece == BlackKnight)
        move = "N";
    else if (fromPiece == WhiteBishop || fromPiece == BlackBishop)
        move = "B";
    else if (fromPiece == WhiteRook || fromPiece == BlackRook)
        move = "R";
    else if (fromPiece == WhiteQueen || fromPiece == BlackQueen)
        move = "Q";
    else if (fromPiece == WhiteKing || fromPiece == BlackKing)
        move = "K";

    move.append(notationAmbiguity(from, to));

    if (takes)
        move.append("x");

    move.append(colToFile(to[1]));
    move.append(rowToRank(to[0]));

    if (promotionPiece != None) {
        move.append("=");

        if (promotionPiece == WhiteKnight || promotionPiece == BlackKnight)
            move.append("N");
        else if (promotionPiece == WhiteBishop || promotionPiece == BlackBishop)
            move.append("B");
        else if (promotionPiece == WhiteRook || promotionPiece == BlackRook)
            move.append("R");
        else if (promotionPiece == WhiteQueen || promotionPiece == BlackQueen)
            move.append("Q");
    }

    if (inCheck(!whiteTurn_, from, to, board_)) {
        Piece boardCopy[8][8];
        copyBoard(board_, boardCopy);
        makeMove(from, to, boardCopy);

        if (!canMove(!whiteTurn_, boardCopy))
            move.append("#");
        else
            move.append("+");
    }

    return move;
}

QString ApplicationGameState::notationAmbiguity(int from[2], int to[2]) const
{
    Piece movingPiece = pieceAt(from, board_);

    bool ambiguousMove = false;
    bool sameRow = false;
    bool sameCol = false;

    int otherPiece[2];
    for (otherPiece[0] = 0; otherPiece[0] < 8; otherPiece[0]++) {
        for (otherPiece[1] = 0; otherPiece[1] < 8; otherPiece[1]++) {
            if (pieceAt(otherPiece, board_) == movingPiece && !equalArrays(from, otherPiece, 2)) {
                if (legalMove(otherPiece, to, whiteTurn_)) {
                    ambiguousMove = true;
                    sameRow = from[0] == otherPiece[0] ? true : sameRow;
                    sameCol = from[1] == otherPiece[1] ? true : sameCol;
                }
            }
        }
    }

    QString string;
    if (ambiguousMove) {
        if (sameRow)
            string = colToFile(from[1]);
        if (sameCol)
            string.append(rowToRank(from[0]));

        if (string.length() == 0)
            string = colToFile(from[1]);
    }

    return string;
}

char ApplicationGameState::colToFile(int col) const
{
    char file = 97 + col;
    return file;
}

char ApplicationGameState::rowToRank(int row) const
{
    char rank = 56 - row;
    return rank;
}
