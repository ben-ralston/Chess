#include "rules.h"

#include "game_state.h"
#include "position.h"
#include "chess/piece.h"

//namespace rules {

//bool inCheck(bool white, const GameState &gameState)
//{
//    Piece king;
//    if (white)
//        king = WhiteKing;
//    else
//        king = BlackKing;

//    bool foundKing = false;
//    int kingPos[2];
//    for (kingPos[0] = 0; kingPos[0] < 8; kingPos[0]++) {
//        for (kingPos[1] = 0; kingPos[1] < 8; kingPos[1]++) {
//            if (gameState.pieceAt(kingPos) == king) {
//                foundKing = true;
//                break;
//            }
//        }
//        if (foundKing)
//            break;
//    }

//    int from[2];
//    for (from[0] = 0; from[0] < 8; from[0]++) {
//        for (from[1] = 0; from[1] < 8; from[1]++) {
//            if (validMove(from, kingPos, !white, true))
//                return true;
//        }
//    }
//    return false;
//}

//bool inCheck(bool white, int from[], int to[])
//{

//}

//bool validBishopMove(int from[2], int to[2], const GameState &gameState)
//{
//    bool white = gameState.pieceAt(from) == WhiteBishop || gameState.pieceAt(from) == WhiteQueen;

//    int rowDiff = to[0] - from[0];
//    int colDiff = to[1] - from[1];

//    if (rowDiff != colDiff && rowDiff != -1 * colDiff)
//        return false;

//    if (!emptySpace(to) && !opponentPiece(to, white))
//        return false;

//    int rowDirection = -1 + 2 * (rowDiff > 0);
//    int colDirection = -1 + 2 * (colDiff > 0);
//    int distance = rowDiff * rowDirection;

//    int row, col;
//    for (int i = 1; i < distance; i++) {
//        row = from[0] + i * rowDirection;
//        col = from[1] + i * colDirection;
//        if (!emptySpace(row, col))
//            return false;
//    }

//    return true;
//}

//bool Position::validKingMove(int from[2], int to[2])
//{
//    bool white = pieceAt(from) == WhiteKing;

//    if (white) {
//        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
//            if (!whiteKingsideCastle_)
//                return false;
//            if (inCheck(true))
//                return false;
//            if (!emptySpace(7, 5) || !emptySpace(7, 6))
//                return false;
//            int middleSquare[2] = { 7, 5 };
//            if (inCheck(white, from, middleSquare))
//                return false;

//            return true;
//        }
//        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
//            if (!whiteQueensideCastle_)
//                return false;
//            if (inCheck(true))
//                return false;
//            if (!emptySpace(7, 1) || !emptySpace(7, 2) || !emptySpace(7, 3))
//                return false;
//            int middleSquare[2] = { 7, 3 };
//            if (inCheck(white, from, middleSquare))
//                return false;

//            return true;
//        }
//    } else {
//        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
//            if (!blackKingsideCastle_)
//                return false;
//            if (inCheck(false))
//                return false;
//            if (!emptySpace(0, 5) || !emptySpace(0, 6))
//                return false;
//            int middleSquare[2] = { 0, 5 };
//            if (inCheck(white, from, middleSquare))
//                return false;

//            return true;
//        }
//        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
//            if (!blackQueensideCastle_)
//                return false;
//            if (inCheck(false))
//                return false;
//            if (!emptySpace(0, 1) || !emptySpace(0, 2) || !emptySpace(0, 3))
//                return false;
//            int middleSquare[2] = { 0, 3 };
//            if (inCheck(white, from, middleSquare))
//                return false;

//            return true;
//        }
//    }

//    int rowDiff = to[0] - from[0];
//    int colDiff = to[1] - from[1];

//    if (rowDiff > 1 || rowDiff < -1 || colDiff > 1 || colDiff < -1)
//        return false;

//    return emptySpace(to) || opponentPiece(to, white);
//}

//bool Position::validKnightMove(int from[2], int to[2]) const
//{
//    bool white = pieceAt(from) == WhiteKnight;

//    if (!emptySpace(to) && !opponentPiece(to, white))
//        return false;

//    bool rowDiff1 = (from[0] - to[0] == 1) || (to[0] - from[0] == 1);
//    bool rowDiff2 = (from[0] - to[0] == 2) || (to[0] - from[0] == 2);
//    bool colDiff1 = (from[1] - to[1] == 1) || (to[1] - from[1] == 1);
//    bool colDiff2 = (from[1] - to[1] == 2) || (to[1] - from[1] == 2);

//    return (rowDiff1 && colDiff2) || (rowDiff2 && colDiff1);
//}

//bool Position::validMove(int from[2], int to[2], bool white, bool allowChecks)
//{
//    if (opponentPiece(from, white))
//        return false;

//    Piece piece = pieceAt(from);
//    bool legalMove;
//    if (piece == WhitePawn || piece == BlackPawn)
//        legalMove = validPawnMove(from, to);
//    else if (piece == WhiteKnight || piece == BlackKnight)
//        legalMove = validKnightMove(from, to);
//    else if (piece == WhiteBishop || piece == BlackBishop)
//        legalMove = validBishopMove(from, to);
//    else if (piece == WhiteRook || piece == BlackRook)
//        legalMove = validRookMove(from, to);
//    else if (piece == WhiteQueen || piece == BlackQueen)
//        legalMove = validQueenMove(from, to);
//    else if (piece == WhiteKing || piece == BlackKing)
//        legalMove = validKingMove(from, to);
//    else
//        return false;

//    if (!legalMove)
//        return false;

//    if (!allowChecks)
//        return !inCheck(white, from, to);
//    else
//        return true;
//}

//bool Position::validPawnMove(int from[2], int to[2]) const
//{
//    bool white = pieceAt(from) == WhitePawn;

//    bool firstMove;
//    int direction;
//    int passantRow;
//    int passantCol;

//    if (white) {
//        firstMove = from[0] == 6;
//        direction = -1;
//        passantRow = 2;
//        passantCol = blackPassantPawn_;
//    }
//    else {
//        firstMove = from[0] == 1;
//        direction = 1;
//        passantRow = 5;
//        passantCol = whitePassantPawn_;
//    }

//    if (firstMove) {
//        if (from[0] + 2 * direction == to[0]) {
//            if (from[1] != to[1])
//                return false;
//            return emptySpace(from[0] + direction, from[1]) && emptySpace(to);
//        }
//    }

//    if (from[0] + direction != to[0])
//        return false;

//    if (from[1] == to[1])
//        return emptySpace(to);

//    if ((to[1] - from[1] == 1) || (from[1] - to[1] == 1)) {
//        if (to[0] == passantRow && to[1] == passantCol)
//            return true;
//        return opponentPiece(to, white);
//    }

//    return false;
//}

//bool Position::validRookMove(int from[2], int to[2]) const
//{
//    bool white = pieceAt(from) == WhiteRook || pieceAt(from) == WhiteQueen;

//    int rowDiff = to[0] - from[0];
//    int colDiff = to[1] - from[1];

//    if (rowDiff != 0 && colDiff != 0)
//        return false;

//    if (!emptySpace(to) && !opponentPiece(to, white))
//        return false;

//    int rowDirection, colDirection, distance;
//    if (rowDiff != 0) {
//        rowDirection = -1 + 2 * (rowDiff > 0);
//        colDirection = 0;
//        distance = rowDiff * rowDirection;
//    } else {
//        rowDirection = 0;
//        colDirection = -1 + 2 * (colDiff > 0);
//        distance = colDiff * colDirection;
//    }

//    int row, col;
//    for (int i = 1; i < distance; i++) {
//        row = from[0] + i * rowDirection;
//        col = from[1] + i * colDirection;
//        if (!emptySpace(row, col))
//            return false;
//    }

//    return true;
//}

//bool Position::validQueenMove(int from[2], int to[2]) const
//{
//    return validBishopMove(from, to) || validRookMove(from, to);
//}

//}
