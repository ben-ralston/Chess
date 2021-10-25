#include "game_state.h"

#include <vector>

#include <QString>

#include "chess/piece.h"
#include "position.h"
#include "move.h"

const Piece GameState::startingPosition_[8][8] = {
    { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook },
    { BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn },
    { None, None, None, None, None, None, None, None },
    { None, None, None, None, None, None, None, None },
    { None, None, None, None, None, None, None, None },
    { None, None, None, None, None, None, None, None },
    { WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn },
    { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook },
};

GameState::GameState()
{
    reset();
}

GameState::GameState(const GameState &a)
{
    copyBoard(a.board_, board_);

    whiteTurn_ = a.whiteTurn_;
    whiteKingsideCastle_ = a.whiteKingsideCastle_;
    whiteQueensideCastle_ = a.whiteQueensideCastle_;
    blackKingsideCastle_ = a.blackKingsideCastle_;
    blackQueensideCastle_ = a.blackQueensideCastle_;
    whitePassantColumn_ = a.whitePassantColumn_;
    blackPassantColumn_ = a.blackPassantColumn_;
    movesNoProgess_ = a.movesNoProgess_;
}

void GameState::getLegalMoves(std::vector<Move> &legalMoves, std::vector<Move> &promotionMoves) const
{
    legalMoves.clear();
    promotionMoves.clear();

    int from[2];
    int to[2];
    for (from[0] = 0; from[0] < 8; from[0]++) {
        for (from[1] = 0; from[1] < 8; from[1]++) {

            if (opponentPiece(from, !whiteTurn_, board_)) {
                for (to[0] = 0; to[0] < 8; to[0]++) {
                    for (to[1] = 0; to[1] < 8; to[1]++) {
                        if (legalMove(from, to, whiteTurn_)) {
                            if (isPromotionMove(from, to))
                                promotionMoves.push_back(Move(from, to));
                            else
                                legalMoves.push_back(Move(from, to));
                        }
                    }
                }
            }
        }
    }
}

Position GameState::savePosition() const
{
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

    bool kingsideCastle;
    bool queensideCastle;
    if (pieceAt(from, board_) != king) {
        kingsideCastle = false;
        queensideCastle = false;
    } else {
        to[1] = 6;
        kingsideCastle = legalMove(from, to, whiteTurn_);

        to[1] = 2;
        queensideCastle = legalMove(from, to, whiteTurn_);
    }

    int passantCol;
    Piece pawn;
    if (whiteTurn_) {
        pawn = WhitePawn;
        passantCol = blackPassantColumn_;
        from[0] = 3;  // White pawns start on 5th rank for en passant
        to[0] = 2;  // Move to 6th rank after en passant
        to[1] = blackPassantColumn_;  // Move to same file where black pawn was
    } else {
        pawn = BlackPawn;
        passantCol = whitePassantColumn_;
        from[0] = 4;  // Black pawns start on 4th rank for en passant
        to[0] = 5;  // Move to 3rd rank after en passant
        to[1] = whitePassantColumn_;  // Move to same file where white pawn was
    }

    int columnToSave;
    if (passantCol == -1)
        columnToSave = passantCol;
    else if (passantCol == 0) {
        from[1] = 1;
        if (legalMove(from, to, whiteTurn_) && pieceAt(from, board_) == pawn)
            columnToSave = passantCol;
        else
            columnToSave = -1;
    } else if (passantCol == 7 && pieceAt(from, board_) == pawn) {
        from[1] = 6;
        if (legalMove(from, to, whiteTurn_))
            columnToSave = passantCol;
        else
            columnToSave = -1;
    } else {
        bool legalPassant = false;

        from[1] = passantCol + 1;
        if (legalMove(from, to, whiteTurn_) && pieceAt(from, board_) == pawn)
            legalPassant = true;
        else {
            from[1] = passantCol - 1;
            if (legalMove(from, to, whiteTurn_) && pieceAt(from, board_) == pawn)
                legalPassant = true;
        }

        columnToSave = legalPassant ? passantCol : -1;
    }

    return Position(board_, whiteTurn_, kingsideCastle, queensideCastle, columnToSave);
}

GameState::VictoryType GameState::getOutcome(const std::vector<const Position> &gameHistory) const
{
    if (!canMove(whiteTurn_, board_)) {
        if (inCheck(whiteTurn_, board_))
            return whiteTurn_ ? BlackCheckmate : WhiteCheckmate;
        else
            return DrawStalemate;
    }

    if (fiftyMoves())
        return DrawFifty;
    if (insufficientMaterial())
        return DrawMaterial;
    if (isRepeat(gameHistory))
        return DrawRepetition;
    return NA;
}

void GameState::reset()
{
    copyBoard(startingPosition_, board_);

    whiteTurn_ = true;
    whiteKingsideCastle_ = true;
    whiteQueensideCastle_ = true;
    blackKingsideCastle_ = true;
    blackQueensideCastle_ = true;
    whitePassantColumn_ = -1;
    blackPassantColumn_ = -1;
    movesNoProgess_ = 0;
}

bool GameState::canMove(bool whiteTurn, const Piece (&board)[8][8]) const
{
    int from[2];
    int to[2];

    for (from[0] = 0; from[0] < 8; from[0]++) {
        for (from[1] = 0; from[1] < 8; from[1]++) {
            for (to[0] = 0; to[0] < 8; to[0]++) {
                for (to[1] = 0; to[1] < 8; to[1]++) {
                    if (legalMove(from, to, whiteTurn, false, board))
                            return true;
                }
            }
        }
    }

    return false;
}

void GameState::copyBoard(const Piece (&original)[8][8], Piece (&copy)[8][8]) const
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            copy[row][col] = original[row][col];
        }
    }
}

bool GameState::equalArrays(int a[], int b[], int len) const
{
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool GameState::inCheck(bool whiteTurn, int from[2], int to[2], const Piece (&board)[8][8]) const
{
    Piece boardCopy[8][8];
    copyBoard(board, boardCopy);

    makeMove(from, to, boardCopy);
    return inCheck(whiteTurn, boardCopy);
}

bool GameState::legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks) const
{
    return legalMove(from, to, whiteTurn, allowChecks, board_);
}

void GameState::makeMove(int from[2], int to[2])
{
    makeMove(from, to, board_);
}

void GameState::makeMove(int from[2], int to[2], Piece (&board)[8][8]) const
{
    if (makeCastleMove(from, to, board))
        return;
    if (makePassantMove(from, to, board))
        return;
    makeStandardMove(from, to, board);
}

void GameState::makeMoveAndUpdate(int from[2], int to[2], Piece promotionPiece)
{
    Piece fromPiece = pieceAt(from, board_);
    Piece toPiece = pieceAt(to, board_);

    if (promotionPiece != None)
        makePromotionMove(from, to, promotionPiece);
    else
        makeMove(from, to);

    updateCastle(from, to);
    updatePassant(from, to, fromPiece);
    updateFiftyMoves(fromPiece, toPiece);

    whiteTurn_ = !whiteTurn_;
}

void GameState::makePromotionMove(int from[], int to[], Piece promotionPiece)
{
    board_[from[0]][from[1]] = None;
    board_[to[0]][to[1]] = promotionPiece;
}

bool GameState::opponentPiece(int pos[2], bool whiteTurn, const Piece (&board)[8][8]) const
{
    if (whiteTurn)
        return pieceAt(pos, board) >= 6;
    else
        return pieceAt(pos, board) >= 0 && pieceAt(pos, board) <= 5;
}

Piece GameState::pieceAt(int pos[2], const Piece (&board)[8][8]) const
{
    return board[pos[0]][pos[1]];
}

void GameState::updateCastle(int from[2], int to[2])
{
    if (from[0] == 7 && from[1] == 4) {
        whiteKingsideCastle_ = false;
        whiteQueensideCastle_ = false;
    }
    if (from[0] == 0 && from[1] == 4) {
        blackKingsideCastle_ = false;
        blackQueensideCastle_ = false;
    }
    if ((from[0] == 7 && from[1] == 7) || (to[0] == 7 && to[1] == 7))
        whiteKingsideCastle_ = false;
    if ((from[0] == 7 && from[1] == 0) || (to[0] == 7 && to[1] == 0))
        whiteQueensideCastle_ = false;
    if ((from[0] == 0 && from[1] == 7) || (to[0] == 0 && to[1] == 7))
        blackKingsideCastle_ = false;
    if ((from[0] == 0 && from[1] == 0) || (to[0] == 0 && to[1] == 0))
        blackQueensideCastle_ = false;
}

void GameState::updateFiftyMoves(Piece fromPiece, Piece toPiece)
{
    if (fromPiece == WhitePawn || fromPiece == BlackPawn || toPiece != None)
        movesNoProgess_ = 0;
    else
        movesNoProgess_++;
}

void GameState::updatePassant(int from[2], int to[2], Piece fromPiece)
{
    if (fromPiece == WhitePawn) {
        if (to[0] - from[0] == -2) {
            whitePassantColumn_ = to[1];
            blackPassantColumn_ = -1;
            return;
        }
    } else if (fromPiece == BlackPawn) {
        if (to[0] - from[0] == 2) {
            whitePassantColumn_ = -1;
            blackPassantColumn_ = to[1];
            return;
        }
    }

    whitePassantColumn_ = -1;
    blackPassantColumn_ = -1;
}

bool GameState::emptySpace(int row, int col, const Piece (&board)[8][8]) const
{
    int pos[2] = { row, col };
    return pieceAt(pos, board) == None;
}

bool GameState::emptySpace(int pos[2], const Piece (&board)[8][8]) const
{
    return pieceAt(pos, board) == None;
}

bool GameState::fiftyMoves() const
{
    if (movesNoProgess_ == 100)
        return true;
    return false;
}

bool GameState::inCheck(bool whiteTurn, const Piece (&board)[8][8]) const
{
    Piece king;
    if (whiteTurn)
        king = WhiteKing;
    else
        king = BlackKing;

    bool foundKing = false;
    int kingPos[2];
    for (kingPos[0] = 0; kingPos[0] < 8; kingPos[0]++) {
        for (kingPos[1] = 0; kingPos[1] < 8; kingPos[1]++) {
            if (pieceAt(kingPos, board) == king) {
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
            if (legalMove(from, kingPos, !whiteTurn, true, board))
                return true;
        }
    }
    return false;
}

bool GameState::insufficientMaterial() const
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

    int pos[2];
    for (pos[0] = 0; pos[0] < 8; pos[0]++) {
        for (pos[1] = 0; pos[1] < 8; pos[1]++) {
            piece = pieceAt(pos, board_);
            if (piece == None)
                continue;

            pieceCount[piece]++;
            lightSquare = (pos[0] + pos[1]) % 2 == 0;

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

    // TODO Expand definition of insufficient material
    // TODO Make arrays const member variables?
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

bool GameState::isPromotionMove(int from[2], int to[2]) const
{
    if (pieceAt(from, board_) == WhitePawn)
        return to[0] == 0;
    else if (pieceAt(from, board_) == BlackPawn)
        return to[0] == 7;
    else
        return false;
}

bool GameState::isRepeat(const std::vector<const Position> &gameHistory) const
{
    // TODO Compare repeat results with chess.com
    const Position &currentPosition = gameHistory.back();

    int repeats = 0;

    // Avoid double counting current position, so stop one iteration early
    std::vector<const Position>::const_iterator endIterator = --(gameHistory.end());
    for (std::vector<const Position>::const_iterator it = gameHistory.begin(); it != endIterator; it++) {
        if (currentPosition == *it)
            if (++repeats == 2)
                return true;
    }

    return false;
}

bool GameState::legalBishopMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    bool whiteTurn = pieceAt(from, board) == WhiteBishop || pieceAt(from, board) == WhiteQueen;

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff != colDiff && rowDiff != -1 * colDiff)
        return false;

    if (!emptySpace(to, board) && !opponentPiece(to, whiteTurn, board))
        return false;

    int rowDirection = -1 + 2 * (rowDiff > 0);
    int colDirection = -1 + 2 * (colDiff > 0);
    int distance = rowDiff * rowDirection;

    int row, col;
    for (int i = 1; i < distance; i++) {
        row = from[0] + i * rowDirection;
        col = from[1] + i * colDirection;
        if (!emptySpace(row, col, board))
            return false;
    }

    return true;
}

bool GameState::legalKingMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    bool whiteTurn = pieceAt(from, board) == WhiteKing;

    if (whiteTurn) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            if (!whiteKingsideCastle_)
                return false;
            if (inCheck(whiteTurn, board))
                return false;
            if (!emptySpace(7, 5, board) || !emptySpace(7, 6, board))
                return false;
            int middleSquare[2] = { 7, 5 };
            if (inCheck(whiteTurn, from, middleSquare, board))
                return false;

            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            if (!whiteQueensideCastle_)
                return false;
            if (inCheck(whiteTurn, board))
                return false;
            if (!emptySpace(7, 1, board) || !emptySpace(7, 2, board) || !emptySpace(7, 3, board))
                return false;
            int middleSquare[2] = { 7, 3 };
            if (inCheck(whiteTurn, from, middleSquare, board))
                return false;

            return true;
        }
    } else {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            if (!blackKingsideCastle_)
                return false;
            if (inCheck(whiteTurn, board))
                return false;
            if (!emptySpace(0, 5, board) || !emptySpace(0, 6, board))
                return false;
            int middleSquare[2] = { 0, 5 };
            if (inCheck(whiteTurn, from, middleSquare, board))
                return false;

            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            if (!blackQueensideCastle_)
                return false;
            if (inCheck(whiteTurn, board))
                return false;
            if (!emptySpace(0, 1, board) || !emptySpace(0, 2, board) || !emptySpace(0, 3, board))
                return false;
            int middleSquare[2] = { 0, 3 };
            if (inCheck(whiteTurn, from, middleSquare, board))
                return false;

            return true;
        }
    }

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff > 1 || rowDiff < -1 || colDiff > 1 || colDiff < -1)
        return false;

    return emptySpace(to, board) || opponentPiece(to, whiteTurn, board);
}

bool GameState::legalKnightMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    bool whiteTurn = pieceAt(from, board) == WhiteKnight;

    if (!emptySpace(to, board) && !opponentPiece(to, whiteTurn, board))
        return false;

    bool rowDiff1 = (from[0] - to[0] == 1) || (to[0] - from[0] == 1);
    bool rowDiff2 = (from[0] - to[0] == 2) || (to[0] - from[0] == 2);
    bool colDiff1 = (from[1] - to[1] == 1) || (to[1] - from[1] == 1);
    bool colDiff2 = (from[1] - to[1] == 2) || (to[1] - from[1] == 2);

    return (rowDiff1 && colDiff2) || (rowDiff2 && colDiff1);
}

bool GameState::legalMove(int from[2], int to[2], bool whiteTurn, bool allowChecks, const Piece (&board)[8][8]) const
{
    if (opponentPiece(from, whiteTurn, board))
        return false;

    Piece piece = pieceAt(from, board);
    bool legalMove;
    if (piece == WhitePawn || piece == BlackPawn)
        legalMove = legalPawnMove(from, to, board);
    else if (piece == WhiteKnight || piece == BlackKnight)
        legalMove = legalKnightMove(from, to, board);
    else if (piece == WhiteBishop || piece == BlackBishop)
        legalMove = legalBishopMove(from, to, board);
    else if (piece == WhiteRook || piece == BlackRook)
        legalMove = legalRookMove(from, to, board);
    else if (piece == WhiteQueen || piece == BlackQueen)
        legalMove = legalQueenMove(from, to, board);
    else if (piece == WhiteKing || piece == BlackKing)
        legalMove = legalKingMove(from, to, board);
    else
        return false;

    if (!legalMove)
        return false;

    if (!allowChecks)
        return !inCheck(whiteTurn, from, to, board);
    else
        return true;
}

bool GameState::legalPawnMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    bool whiteTurn = pieceAt(from, board) == WhitePawn;

    bool firstMove;
    int direction;
    int passantRow;
    int passantCol;

    if (whiteTurn) {
        firstMove = from[0] == 6;
        direction = -1;
        passantRow = 2;
        passantCol = blackPassantColumn_;
    }
    else {
        firstMove = from[0] == 1;
        direction = 1;
        passantRow = 5;
        passantCol = whitePassantColumn_;
    }

    if (firstMove) {
        if (from[0] + 2 * direction == to[0]) {
            if (from[1] != to[1])
                return false;
            return emptySpace(from[0] + direction, from[1], board) && emptySpace(to, board);
        }
    }

    if (from[0] + direction != to[0])
        return false;

    if (from[1] == to[1])
        return emptySpace(to, board);

    if ((to[1] - from[1] == 1) || (from[1] - to[1] == 1)) {
        if (to[0] == passantRow && to[1] == passantCol)
            return true;
        return opponentPiece(to, whiteTurn, board);
    }

    return false;
}

bool GameState::legalRookMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    bool white = pieceAt(from, board) == WhiteRook || pieceAt(from, board) == WhiteQueen;

    int rowDiff = to[0] - from[0];
    int colDiff = to[1] - from[1];

    if (rowDiff != 0 && colDiff != 0)
        return false;

    if (!emptySpace(to, board) && !opponentPiece(to, white, board))
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
        if (!emptySpace(row, col, board))
            return false;
    }

    return true;
}

bool GameState::legalQueenMove(int from[2], int to[2], const Piece (&board)[8][8]) const
{
    return legalBishopMove(from, to, board) || legalRookMove(from, to, board);
}

bool GameState::makeCastleMove(int from[2], int to[2], Piece (&board)[8][8]) const
{
    // TODO Find better method for comparisons
    // TODO Maybe save pertinent values as variables

    if (pieceAt(from, board) == WhiteKing) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            int rookFrom[2] = {7, 7};
            int rookTo[2] = {7, 5};
            makeStandardMove(from, to, board);
            makeStandardMove(rookFrom, rookTo, board);
            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            int rookFrom[2] = {7, 0};
            int rookTo[2] = {7, 3};
            makeStandardMove(from, to, board);
            makeStandardMove(rookFrom, rookTo, board);
            return true;
        }
    }

    if (pieceAt(from, board) == BlackKing) {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            int rookFrom[2] = {0, 7};
            int rookTo[2] = {0, 5};
            makeStandardMove(from, to, board);
            makeStandardMove(rookFrom, rookTo, board);
            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            int rookFrom[2] = {0, 0};
            int rookTo[2] = {0, 3};
            makeStandardMove(from, to, board);
            makeStandardMove(rookFrom, rookTo, board);
            return true;
        }
    }

    return false;
}

bool GameState::makePassantMove(int from[2], int to[2], Piece (&board)[8][8]) const
{
    int removeDirection;
    int passantRow;
    int passantCol;

    if (pieceAt(from, board) == WhitePawn) {
        removeDirection = 1;
        passantRow = 2;
        passantCol = blackPassantColumn_;
    } else if (pieceAt(from, board) == BlackPawn) {
        removeDirection = -1;
        passantRow = 5;
        passantCol = whitePassantColumn_;
    } else
        return false;

    if (to[0] == passantRow && to[1] == passantCol) {
        makeStandardMove(from, to, board);
        board[passantRow + removeDirection][passantCol] = None;
        return true;
    }

    return false;
}

void GameState::makeStandardMove(int from[2], int to[2], Piece (&board)[8][8]) const
{
    Piece movingPiece = pieceAt(from, board_);
    board[from[0]][from[1]] = None;
    board[to[0]][to[1]] = movingPiece;
}
