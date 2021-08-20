#include "game.h"

#include <array>
#include <algorithm>
#include <iostream>

#include <QKeyEvent>

using namespace std;

Game::Game(QObject *parent)
    : QObject(parent), selected_{-1, -1}
{
    resetGame();
}

Game::~Game() {

}

void Game::updatePosition()
{
    Position position = gameHistory_[shownMoveNumber_];
    Piece piece;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            piece = position.board[indexAdjustment(row)]
                                  [indexAdjustment(col)];
            emit setPiece(row, col, piece);
        }
    }
}

void Game::expiredTime(bool white)
{
    // TODO Implement variable behavior based on winning color
    resetGame();
}

void Game::keyPress(int key)
{
    if (key == Qt::Key_Down) {
        if (shownMoveNumber_ == 0)
           return;
        shownMoveNumber_ = 0;
    } else if (key == Qt::Key_Left) {
        if (shownMoveNumber_ == 0)
            return;
        shownMoveNumber_--;
    } else if (key == Qt::Key_Right) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_++;
    } else if (key == Qt::Key_Up) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_ = trueMoveNumber_;
    }

    clearSelectedSquare();
    updatePosition();
}

void Game::mousePress(int row, int col)
{
    if (shownMoveNumber_ != trueMoveNumber_)
        return;

    int adjRow = indexAdjustment(row);
    int adjCol = indexAdjustment(col);
    bool selectable = isSelectable(adjRow, adjCol);

    if (selected_[0] == row && selected_[1] == col) {
        clearSelectedSquare();
        return;
    }
    if (selectable) {
        setSelectedSquare(row, col);
        return;
    }
    if (selected_[0] == -1)
        return;

    int from[2] = {indexAdjustment(selected_[0]),
                   indexAdjustment(selected_[1])};
    int to[2] = {adjRow, adjCol};

    if (!validMove(from, to, whiteTurn_)) {
        clearSelectedSquare();
        return;
    }

    Piece fromPiece = pieceAt(from);
    Piece toPiece = pieceAt(to);

    makeMove(from, to);
    updateCastle(from, to);
    updatePassant(from, to, fromPiece);
    updateFiftyMoves(fromPiece, toPiece);
    whiteTurn_ = !whiteTurn_;

    if (isCheckMate()) {
        resetGame();
        cout << "Win!\n";
        return;
    }
    if (isDraw()) {
        resetGame();
        cout << "Draw!\n";
        return;
    }

    shownMoveNumber_++;
    trueMoveNumber_++;
    clearSelectedSquare();
    updatePosition();
}

void Game::resetGame()
{
    whiteTurn_ = true;
    clearSelectedSquare();
    shownMoveNumber_ = 0;
    trueMoveNumber_ = 0;

    whiteKingsideCastle_ = true;
    whiteQueensideCastle_ = true;
    blackKingsideCastle_ = true;
    blackQueensideCastle_ = true;
    whitePassantPawn_ = -1;
    blackPassantPawn_ = -1;
    movesNoProgess_ = 0;

    gameHistory_.clear();
    repeatPositions_.clear();
    setStartingPosition();
    gameHistory_.push_back(savePosition());

    updatePosition();
}

bool Game::canMove()
{
    int from[2];
    int to[2];

    for (from[0] = 0; from[0] < 8; from[0]++) {
        for (from[1] = 0; from[1] < 8; from[1]++) {
            for (to[0] = 0; to[0] < 8; to[0]++) {
                for (to[1] = 0; to[1] < 8; to[1]++) {
                    if (validMove(from, to, whiteTurn_))
                            return true;
                }
            }
        }
    }

    return false;
}

void Game::clearSelectedSquare()
{
    if (selected_[0] == -1)
        return;

    emit highlightSquare(selected_);
    selected_[0] = -1;
    selected_[1] = -1;
}

void Game::copyBoard(const Piece *original, Piece *copy)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            copy[8 * row + col] = original[8 * row + col];
        }
    }
}

bool Game::emptySpace(int row, int col) const
{
    return pieceAt(row, col) == None;
}

bool Game::emptySpace(int pos[2]) const
{
    return emptySpace(pos[0], pos[1]);
}

bool Game::equalArrays(int a[], int b[], int len) const
{
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool Game::fiftyMoves() const
{
    if (movesNoProgess_ == 100)
        return true;
    return false;
}

bool Game::inCheck(bool white)
{
    Piece king;
    if (white)
        king = WhiteKing;
    else
        king = BlackKing;

    bool foundKing = false;
    int kingPos[2];
    for (kingPos[0] = 0; kingPos[0] < 8; kingPos[0]++) {
        for (kingPos[1] = 0; kingPos[1] < 8; kingPos[1]++) {
            if (pieceAt(kingPos) == king) {
                foundKing = true;
                break;
            }
        }
        if (foundKing)
            break;
    }

    int from[2];
    for (from[0] = 0; from[0] < 8; from[0]++) {
        for (from[1] = 0; from[1] < 8; from[1]++) {
            if (validMove(from, kingPos, !white, true))
                return true;
        }
    }
    return false;
}

bool Game::inCheck(bool white, int from[2], int to[2])
{
    Piece boardCopy[8][8];
    copyBoard(&position_[0][0], &boardCopy[0][0]);

    makeMove(from, to);
    bool check = inCheck(white);

    copyBoard(&boardCopy[0][0], &position_[0][0]);

    return check;
}

int Game::indexAdjustment(int rowOrColIndex) const
{
    return whiteTurn_ ? rowOrColIndex : 7 - rowOrColIndex;
}

bool Game::insufficientMaterial() const
{
    int pieceCount[12];
    for (int i = 0; i < 12; i++) {
        pieceCount[i] = 0;
    }

    bool whiteLightBishop = false;
    bool whiteDarkBishop = false;
    bool blackLightBishop = false;
    bool blackDarkBishop = false;
    bool lightSquare;
    Piece piece;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            piece = pieceAt(row, col);
            if (piece == None)
                continue;

            pieceCount[piece]++;
            lightSquare = (row + col) % 2 == 0;

            if (piece == WhiteBishop) {
                if (lightSquare)
                    whiteLightBishop = true;
                else
                    whiteDarkBishop = true;
            } else if (piece == BlackBishop) {
                if (lightSquare)
                    blackLightBishop = true;
                else
                    blackDarkBishop = true;
            }
        }
    }

    int kingVsKing[12] = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
    int kingVsWhiteKnight[12] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
    int kingVsWhiteBishop[12] = { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
    int kingVsBlackKnight[12] = { 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1 };
    int kingVsBlackBishop[12] = { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1 };
    int bishopsOnlyIndex[8] = { 0, 1, 3, 4, 6, 7, 9, 10 };

    if (equalArrays(pieceCount, kingVsKing, 12))
        return true;
    if (equalArrays(pieceCount, kingVsWhiteKnight, 12))
        return true;
    if (equalArrays(pieceCount, kingVsWhiteBishop, 12))
        return true;
    if (equalArrays(pieceCount, kingVsBlackKnight, 12))
        return true;
    if (equalArrays(pieceCount, kingVsBlackBishop, 12))
        return true;

    int index;
    for (int i = 0; i < 8; i++) {
        index = bishopsOnlyIndex[i];
        if (pieceCount[index] != 0)
            return false;
    }

    // Only remaining pieces are kings and bishops

    if (!(whiteLightBishop || blackLightBishop))
        return true;
    if (!(whiteDarkBishop || blackDarkBishop))
        return true;

    return false;
}

bool Game::isCheckMate()
{
    if (!inCheck(whiteTurn_))
        return false;
    return !canMove();
}

bool Game::isDraw()
{
    return fiftyMoves() || insufficientMaterial() || isRepeat() || isStalemate();
}

bool Game::isRepeat()
{
    Position currentPosition = savePosition();

    for (size_t i = 0; i < repeatPositions_.size(); i++) {
        if (currentPosition == repeatPositions_[i]) {
            gameHistory_.push_back(currentPosition);
            return true;
        }
    }

    // TODO Compare repeat results with chess.com
    for (size_t i = 1; i < gameHistory_.size(); i++) {
        if (currentPosition == gameHistory_[i]) {
            repeatPositions_.push_back(currentPosition);
            break;
        }
    }

    gameHistory_.push_back(currentPosition);
    return false;
}

bool Game::isSelectable(int row, int col) const
{
    Piece piece = pieceAt(row, col);

    if (whiteTurn_)
        return piece >= 0 && piece <= 5;
    else
        return piece >= 6;
}

bool Game::isStalemate()
{
    if (inCheck(whiteTurn_))
        return false;
    return !canMove();
}

bool Game::makeCastleMove(int from[2], int to[2])
{
    // TODO Find better method for comparisons
    // TODO Maybe save pertinent values as constants

    if (pieceAt(from) == WhiteKing) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            int rookFrom[2] = {7, 7};
            int rookTo[2] = {7, 5};
            makeStandardMove(from, to);
            makeStandardMove(rookFrom, rookTo);
            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            int rookFrom[2] = {7, 0};
            int rookTo[2] = {7, 3};
            makeStandardMove(from, to);
            makeStandardMove(rookFrom, rookTo);
            return true;
        }
    }

    if (pieceAt(from) == BlackKing) {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            int rookFrom[2] = {0, 7};
            int rookTo[2] = {0, 5};
            makeStandardMove(from, to);
            makeStandardMove(rookFrom, rookTo);
            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            int rookFrom[2] = {0, 0};
            int rookTo[2] = {0, 3};
            makeStandardMove(from, to);
            makeStandardMove(rookFrom, rookTo);
            return true;
        }
    }

    return false;
}

void Game::makeMove(int from[2], int to[2])
{
    if (makeCastleMove(from, to))
        return;
    if (makePassantMove(from, to))
        return;
    if (makePromotionMove(from, to, true))
        return;
    makeStandardMove(from, to);
}


bool Game::makePassantMove(int from[2], int to[2])
{
    int removeDirection;
    int passantRow;
    int passantCol;

    if (pieceAt(from) == WhitePawn) {
        removeDirection = 1;
        passantRow = 2;
        passantCol = blackPassantPawn_;
    } else if (pieceAt(to) == BlackPawn) {
        removeDirection = -1;
        passantRow = 5;
        passantCol = whitePassantPawn_;
    } else
        return false;

    if (to[0] == passantRow && to[1] == passantCol) {
        makeStandardMove(from, to);
        position_[passantRow + removeDirection][passantCol] = None;
        return true;
    }

    return false;
}

bool Game::makePromotionMove(int from[2], int to[2], bool autoQueen) {
    int promotionRow;
    Piece promotionPiece;

    if (pieceAt(from) == WhitePawn) {
        promotionRow = 0;
        promotionPiece = WhiteQueen;
    } else if (pieceAt(from) == BlackPawn) {
        promotionRow = 7;
        promotionPiece = BlackQueen;
    } else
        return false;

    if (to[0] == promotionRow) {
        position_[from[0]][from[1]] = None;
        position_[to[0]][to[1]] = promotionPiece;
        return true;
    }

    return false;
}

void Game::makeStandardMove(int from[2], int to[2]) {
    Piece movingPiece = pieceAt(from);
    position_[from[0]][from[1]] = None;
    position_[to[0]][to[1]] = movingPiece;
}

bool Game::opponentPiece(int r, int c, bool white) const
{
    if (white)
        return pieceAt(r, c) >= 6;
    else
        return pieceAt(r, c) >= 0 && pieceAt(r, c) <= 5;
}

bool Game::opponentPiece(int pos[2], bool white) const
{
    return opponentPiece(pos[0], pos[1], white);
}

Piece Game::pieceAt(int row, int col) const
{
    return position_[row][col];
}

Piece Game::pieceAt(int pos[2]) const
{
    return pieceAt(pos[0], pos[1]);
}

Position Game::savePosition()
{
    Position current;
    copyBoard(&position_[0][0], &current.board[0][0]);

    current.whiteTurn = whiteTurn_;

//    current.kingsideCastle = true;
//    current.queensideCastle = true;

    Piece king;
    int from[2];
    int to[2];
    if (whiteTurn_) {
        king = WhiteKing;
        from[0] = 7;
        to[0] = 7;
    } else {
        king = BlackKing;
        from[0] = 0;
        to[0] = 0;
    }

    from[1] = 4;

    if (pieceAt(from) != king) {
        current.kingsideCastle = false;
        current.queensideCastle = false;
    } else {
        to[1] = 6;
        current.kingsideCastle = validMove(from, to, whiteTurn_);

        to[1] = 2;
        current.queensideCastle = validMove(from, to, whiteTurn_);
    }

//    current.enPassantColumn = -1;

    int passantCol;
    if (whiteTurn_) {
        passantCol = blackPassantPawn_;
        from[0] = 3;  // White pawns start on 5th rank for en passant
        to[0] = 2;  // Move to 6th rank after en passant
        to[1] = blackPassantPawn_;  // Move to same file where black pawn was
    } else {
        passantCol = whitePassantPawn_;
        from[0] = 4;  // Black pawns start on 4th rank for en passant
        to[0] = 5;  // Move to 3rd rank after en passant
        to[1] = whitePassantPawn_;  // Move to same file where white pawn was
    }

    if (passantCol == -1)
        current.enPassantColumn = passantCol;
    else if (passantCol == 0) {
        from[1] = 1;
        if (validMove(from, to, whiteTurn_))
            current.enPassantColumn = passantCol;
        else
            current.enPassantColumn = -1;
    }
    else if (passantCol == 7) {
        from[1] = 6;
        if (validMove(from, to, whiteTurn_))
            current.enPassantColumn = passantCol;
        else
            current.enPassantColumn = -1;
    } else {
        bool legalPassant = false;

        from[1] = passantCol + 1;
        if (validMove(from, to, whiteTurn_))
            legalPassant = true;
        else {
            from[1] = passantCol - 1;
            if (validMove(from, to, whiteTurn_))
                legalPassant = true;
        }

        current.enPassantColumn = legalPassant ? passantCol : -1;
    }

    return current;
}

void Game::setSelectedSquare(int row, int col)
{
    clearSelectedSquare();
    selected_[0] = row;
    selected_[1] = col;
    emit highlightSquare(selected_);
}

void Game::setStartingPosition()
{
    copyBoard(&startingPosition_[0][0], &position_[0][0]);
}

void Game::updateCastle(int from[2], int to[2])
{
    if (from[0] == 7 && from[1] == 4) {
        whiteKingsideCastle_ = false;
        whiteQueensideCastle_ = false;
    }
    if (from[0] == 0 && from[1] == 4) {
        blackKingsideCastle_ = false;
        blackQueensideCastle_ = false;
    }
    if ((from[0] == 7 && from[1] == 7) || (to[0] == 7 && to[1] == 7)) {
        whiteKingsideCastle_ = false;
    }
    if ((from[0] == 7 && from[1] == 0) || (to[0] == 7 && to[1] == 0)) {
        whiteQueensideCastle_ = false;
    }
    if ((from[0] == 0 && from[1] == 7) || (to[0] == 0 && to[1] == 7)) {
        blackKingsideCastle_ = false;
    }
    if ((from[0] == 0 && from[1] == 0) || (to[0] == 0 && to[1] == 0)) {
        blackQueensideCastle_ = false;
    }
}

void Game::updateFiftyMoves(Piece fromPiece, Piece toPiece)
{
    if (fromPiece == WhitePawn || fromPiece == BlackPawn || toPiece != None)
        movesNoProgess_ = 0;
    else
        movesNoProgess_++;
}

void Game::updatePassant(int from[2], int to[2], Piece fromPiece)
{
    if (fromPiece == WhitePawn) {
        if (to[0] - from[0] == -2) {
            whitePassantPawn_ = to[1];
            blackPassantPawn_ = -1;
            return;
        }
    } else if (fromPiece == BlackPawn) {
        if (to[0] - from[0] == 2) {
            whitePassantPawn_ = -1;
            blackPassantPawn_ = to[1];
            return;
        }
    }

    whitePassantPawn_ = -1;
    blackPassantPawn_ = -1;
}

bool Game::validBishopMove(int from[2], int to[2]) const
{
    bool white = pieceAt(from) == WhiteBishop || pieceAt(from) == WhiteQueen;

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff != colDiff && rowDiff != -1 * colDiff)
        return false;

    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    int rowDirection = -1 + 2 * (rowDiff > 0);
    int colDirection = -1 + 2 * (colDiff > 0);
    int distance = rowDiff * rowDirection;

    int row, col;
    for (int i = 1; i < distance; i++) {
        row = from[0] + i * rowDirection;
        col = from[1] + i * colDirection;
        if (!emptySpace(row, col))
            return false;
    }

    return true;
}

bool Game::validKingMove(int from[2], int to[2])
{
    bool white = pieceAt(from) == WhiteKing;

    // TODO Check for passing through check
    if (white) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            if (!whiteKingsideCastle_)
                return false;
            if (inCheck(true))
                return false;
            if (!emptySpace(7, 5) || !emptySpace(7, 6))
                return false;

            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            if (!whiteQueensideCastle_)
                return false;
            if (inCheck(true))
                return false;
            if (!emptySpace(7, 1) || !emptySpace(7, 2) || !emptySpace(7, 3))
                return false;

            return true;
        }
    } else {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            if (!blackKingsideCastle_)
                return false;
            if (inCheck(false))
                return false;
            if (!emptySpace(0, 5) || !emptySpace(0, 6))
                return false;

            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            if (!blackQueensideCastle_)
                return false;
            if (inCheck(false))
                return false;
            if (!emptySpace(0, 1) || !emptySpace(0, 2) || !emptySpace(0, 3))
                return false;

            return true;
        }
    }

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff > 1 || rowDiff < -1 || colDiff > 1 || colDiff < -1)
        return false;

    return emptySpace(to) || opponentPiece(to, white);
}

bool Game::validKnightMove(int from[2], int to[2]) const
{
    bool white = pieceAt(from) == WhiteKnight;

    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    bool rowDiff1 = (from[0] - to[0] == 1) || (to[0] - from[0] == 1);
    bool rowDiff2 = (from[0] - to[0] == 2) || (to[0] - from[0] == 2);
    bool colDiff1 = (from[1] - to[1] == 1) || (to[1] - from[1] == 1);
    bool colDiff2 = (from[1] - to[1] == 2) || (to[1] - from[1] == 2);

    return (rowDiff1 && colDiff2) || (rowDiff2 && colDiff1);
}

bool Game::validMove(int from[2], int to[2], bool white, bool allowChecks)
{
    if (opponentPiece(from, white))
        return false;

    Piece piece = pieceAt(from);
    bool legalMove;
    if (piece == WhitePawn || piece == BlackPawn)
        legalMove = validPawnMove(from, to);
    else if (piece == WhiteKnight || piece == BlackKnight)
        legalMove = validKnightMove(from, to);
    else if (piece == WhiteBishop || piece == BlackBishop)
        legalMove = validBishopMove(from, to);
    else if (piece == WhiteRook || piece == BlackRook)
        legalMove = validRookMove(from, to);
    else if (piece == WhiteQueen || piece == BlackQueen)
        legalMove = validQueenMove(from, to);
    else if (piece == WhiteKing || piece == BlackKing)
        legalMove = validKingMove(from, to);
    else
        return false;

    if (!legalMove)
        return false;

    if (!allowChecks)
        return !inCheck(white, from, to);
    else
        return true;
}

bool Game::validPawnMove(int from[2], int to[2]) const
{
    bool white = pieceAt(from) == WhitePawn;

    bool firstMove;
    int direction;
    int passantRow;
    int passantCol;

    if (white) {
        firstMove = from[0] == 6;
        direction = -1;
        passantRow = 2;
        passantCol = blackPassantPawn_;
    }
    else {
        firstMove = from[0] == 1;
        direction = 1;
        passantRow = 5;
        passantCol = whitePassantPawn_;
    }

    if (firstMove) {
        if (from[0] + 2 * direction == to[0]) {
            if (from[1] != to[1])
                return false;
            return emptySpace(from[0] + direction, from[1]) && emptySpace(to);
        }
    }

    if (from[0] + direction != to[0])
        return false;

    if (from[1] == to[1])
        return emptySpace(to);

    if ((to[1] - from[1] == 1) || (from[1] - to[1] == 1)) {
        if (to[0] == passantRow && to[1] == passantCol)
            return true;
        return opponentPiece(to, white);
    }

    return false;
}

bool Game::validRookMove(int from[2], int to[2]) const
{
    bool white = pieceAt(from) == WhiteRook || pieceAt(from) == WhiteQueen;

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff != 0 && colDiff != 0)
        return false;

    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    int rowDirection, colDirection, distance;
    if (rowDiff != 0) {
        rowDirection = -1 + 2 * (rowDiff > 0);
        colDirection = 0;
        distance = rowDiff * rowDirection;
    } else {
        rowDirection = 0;
        colDirection = -1 + 2 * (colDiff > 0);
        distance = colDiff * colDirection;
    }

    int row, col;
    for (int i = 1; i < distance; i++) {
        row = from[0] + i * rowDirection;
        col = from[1] + i * colDirection;
        if (!emptySpace(row, col))
            return false;
    }

    return true;
}

bool Game::validQueenMove(int from[2], int to[2]) const
{
    return validBishopMove(from, to) || validRookMove(from, to);
}
