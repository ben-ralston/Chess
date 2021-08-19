#include "game.h"

#include <array>

using namespace std;

Game::Game()
{
    resetGame();
}

Game::~Game() {

}

void Game::resetGame()
{
    whiteTurn_ = true;
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
}

int Game::tryMove(int from[2], int to[2])
{
    if (!whiteTurn_) {
        from[0] = 7 - from[0];
        from[1] = 7 - from[1];
        to[0] = 7 - to[0];
        to[1] = 7 - to[1];
    }

    if (!validMove(from, to))
        return 0;
    if (inCheck(whiteTurn_, from, to))
        return 0;

    Piece fromPiece = position_[from[0]][from[1]];
    Piece toPiece = position_[to[0]][to[1]];
    makeMove(from, to);
    updateCastle(from, to);
    updatePassant(from, to, fromPiece);
    updateFiftyMoves(fromPiece, toPiece);
    whiteTurn_ = !whiteTurn_;

    if (isCheckMate()) {
        resetGame();
        cout << "Win!\n";
        return 2;
    }
    if (isDraw()) {
        resetGame();
        cout << "Draw!\n";
        return 2;
    }

    return 1;
}

Position Game::getPosition(int moveNum) const
{
    if (moveNum == -1)
        return gameHistory_[gameHistory_.size() - 1];

    return gameHistory_[moveNum];
}

bool Game::isSelectable(int row, int col) const
{
    if (whiteTurn_) {
        if (position_[row][col] >= 0 && position_[row][col] <= 5)
            return true;
        else
            return false;
    } else {
        if (position_[7 - row][7 - col] >= 6)
            return true;
        else
            return false;
    }
}















void Game::setStartingPosition() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            position_[row][col] = startingPosition_[row][col];
        }
    }
}

Position Game::savePosition() {
    Position current;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            current.board[r][c] = position_[r][c];
        }
    }

    current.whiteTurn = whiteTurn_;

    int from[2], to[2];
    if (whiteTurn_) {
        from[0] = 7;
        from[1] = 4;
        if (position_[from[0]][from[1]] != WhiteKing) {
            current.kingsideCastle = false;
            current.queensideCastle = false;
        } else {
            to[0] = 7;
            to[1] = 6;
            current.kingsideCastle = validMove(from, to);

            to[0] = 7;
            to[1] = 2;
            current.queensideCastle = validMove(from, to);
        }
    } else {
        from[0] = 0;
        from[1] = 4;
        if (position_[from[0]][from[1]] != WhiteKing) {
            current.kingsideCastle = false;
            current.queensideCastle = false;
        } else {
            to[0] = 0;
            to[1] = 6;
            current.kingsideCastle = validMove(from, to);

            to[0] = 0;
            to[1] = 2;
            current.queensideCastle = validMove(from, to);
        }
    }

    int col;
    int row;
    Piece pawn;
    if (whiteTurn_) {
        col = blackPassantPawn_;
        row = 3;
        pawn = WhitePawn;
    } else {
        col = whitePassantPawn_;
        row = 4;
        pawn = BlackPawn;
    }

    if (col == -1)
        current.enPassantColumn = col;
    else if (col == 0) {
        if (position_[row][1] == pawn)
            current.enPassantColumn = col;
        else
            current.enPassantColumn = -1;
    }
    else if (col == 7) {
        if (position_[row][6] == pawn)
            current.enPassantColumn = col;
        else
            current.enPassantColumn = -1;
    } else {
        if (position_[row][col - 1] == pawn || position_[row][col + 1] == pawn)
            current.enPassantColumn = col;
        else
            current.enPassantColumn = -1;
    }

    return current;
}

void Game::makeMove(int from[2], int to[2]) {
    if (makeCastleMove(from, to))
        return;
    if (makePassantMove(from, to))
        return;
    if (makePromotionMove(from, to, true))
        return;
    makeStandardMove(from, to);
}

void Game::makeStandardMove(int from[2], int to[2]) {
    Piece toMove = position_[from[0]][from[1]];
    position_[from[0]][from[1]] = None;
    position_[to[0]][to[1]] = toMove;
}

bool Game::makeCastleMove(int from[2], int to[2]) {

    if (position_[from[0]][from[1]] == WhiteKing) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            int r_from[2] = {7, 7};
            int r_to[2] = {7, 5};
            makeStandardMove(from, to);
            makeStandardMove(r_from, r_to);
            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            int r_from[2] = {7, 0};
            int r_to[2] = {7, 3};
            makeStandardMove(from, to);
            makeStandardMove(r_from, r_to);
            return true;
        }
    }

    if (position_[from[0]][from[1]] == BlackKing) {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            int r_from[2] = {0, 7};
            int r_to[2] = {0, 5};
            makeStandardMove(from, to);
            makeStandardMove(r_from, r_to);
            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            int r_from[2] = {0, 0};
            int r_to[2] = {0, 3};
            makeStandardMove(from, to);
            makeStandardMove(r_from, r_to);
            return true;
        }
    }

    return false;
}

bool Game::makePassantMove(int from[2], int to[2]) {
    int removeDirection;
    int passantRow;
    int passantCol;

    if (position_[from[0]][from[1]] == WhitePawn) {
        removeDirection = 1;
        passantRow = 2;
        passantCol = blackPassantPawn_;

    } else if (position_[from[0]][from[1]] == BlackPawn) {
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
    int promRow;
    Piece promPiece;

    if (position_[from[0]][from[1]] == WhitePawn) {
        promRow = 0;
        promPiece = WhiteQueen;
    } else if (position_[from[0]][from[1]] == BlackPawn) {
        promRow = 7;
        promPiece = BlackQueen;
    } else
        return false;

    if (to[0] == promRow) {
        position_[from[0]][from[1]] = None;
        position_[to[0]][to[1]] = promPiece;
        return true;
    }

    return false;
}

bool Game::validMove(int from[2], int to[2]) {
    switch (position_[from[0]][from[1]]) {
    case WhitePawn:
        return validPawnMove(from, to, true);
    case BlackPawn:
        return validPawnMove(from, to, false);
    case WhiteKnight:
        return validKnightMove(from, to, true);
    case BlackKnight:
        return validKnightMove(from, to, false);
    case WhiteBishop:
        return validBishopMove(from, to, true);
    case BlackBishop:
        return validBishopMove(from, to, false);
    case WhiteRook:
        return validRookMove(from, to, true);
    case BlackRook:
        return validRookMove(from, to, false);
    case WhiteQueen:
        return validQueenMove(from, to, true);
    case BlackQueen:
        return validQueenMove(from, to, false);
    case WhiteKing:
        return validKingMove(from, to, true);
    case BlackKing:
        return validKingMove(from, to, false);
    case None:
        return false;
    }
}

bool Game::validPawnMove(int from[2], int to[2], bool white) {
    int direction;
    bool firstMove;
    int passantRow;
    int passantCol;

    if (white) {
        direction = -1;
        firstMove = from[0] == 6;
        passantRow = 2;
        passantCol = blackPassantPawn_;
    }
    else {
        direction = 1;
        firstMove = from[0] == 1;
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

    if (to[1] - from[1] == 1 || from[1] - to[1] == 1){
        if (to[0] == passantRow && to[1] == passantCol)
            return true;
        return opponentPiece(to, white);
    }

    return false;
}

bool Game::validKnightMove(int from[2], int to[2], bool white) {
    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    bool vert1 = (from[0] - to[0] == 1) || (to[0] - from[0] == 1);
    bool vert2 = (from[0] - to[0] == 2) || (to[0] - from[0] == 2);
    bool horz1 = (from[1] - to[1] == 1) || (to[1] - from[1] == 1);
    bool horz2 = (from[1] - to[1] == 2) || (to[1] - from[1] == 2);
    return (vert1 && horz2) || (vert2 && horz1);
}

bool Game::validBishopMove(int from[2], int to[2], bool white) {
    int r_diff = to[0] - from[0];
    int c_diff = to[1] - from[1];

    if (r_diff != c_diff && r_diff != -1 * c_diff)
        return false;

    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    int r_dir = -1 + 2 * (r_diff > 0);
    int c_dir = -1 + 2 * (c_diff > 0);
    int dist = r_diff * r_dir;

    int r, c;
    for (int i = 1; i < dist; i++) {
        r = from[0] + i * r_dir;
        c = from[1] + i * c_dir;
        if (!emptySpace(r, c))
            return false;
    }

    return true;
}

bool Game::validRookMove(int from[2], int to[2], bool white) {
    int r_diff = to[0] - from[0];
    int c_diff = to[1] - from[1];

    if (r_diff != 0 && c_diff != 0)
        return false;

    if (!emptySpace(to) && !opponentPiece(to, white))
        return false;

    int r_dir, c_dir, dist;
    if (r_diff != 0) {
        r_dir = -1 + 2 * (r_diff > 0);
        c_dir = 0;
        dist = r_diff * r_dir;
    } else {
        r_dir = 0;
        c_dir = -1 + 2 * (c_diff > 0);
        dist = c_diff * c_dir;
    }

    int r, c;
    for (int i = 1; i < dist; i++) {
        r = from[0] + i * r_dir;
        c = from[1] + i * c_dir;
        if (!emptySpace(r, c))
            return false;
    }

    return true;
}

bool Game::validQueenMove(int from[2], int to[2], bool white) {
    return validBishopMove(from, to, white) || validRookMove(from, to, white);
}

bool Game::validKingMove(int from[2], int to[2], bool white) {
    if (white) {
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 6) {
            if (!whiteKingsideCastle_)
                return false;
            if (inCheck(true))
                return false;
            if (!emptySpace(7, 5))
                return false;
            if (!emptySpace(7, 6))
                return false;

            return true;
        }
        if (from[0] == 7 && from[1] == 4 && to[0] == 7 && to[1] == 2) {
            if (!whiteQueensideCastle_)
                return false;
            if (inCheck(true))
                return false;
            if (!emptySpace(7, 1))
                return false;
            if (!emptySpace(7, 2))
                return false;
            if (!emptySpace(7, 3))
                return false;

            return true;
        }
    } else {
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 6) {
            if (!blackKingsideCastle_)
                return false;
            if (inCheck(false))
                return false;
            if (!emptySpace(0, 5))
                return false;
            if (!emptySpace(0, 6))
                return false;

            return true;
        }
        if (from[0] == 0 && from[1] == 4 && to[0] == 0 && to[1] == 2) {
            if (!blackQueensideCastle_)
                return false;
            if (inCheck(false))
                return false;
            if (!emptySpace(0, 1))
                return false;
            if (!emptySpace(0, 2))
                return false;
            if (!emptySpace(0, 3))
                return false;

            return true;
        }
    }

    int r_diff = to[0] - from[0];
    int c_diff = to[1] - from[1];

    if (r_diff > 1 || r_diff < -1 || c_diff > 1 || c_diff < -1)
        return false;

    return emptySpace(to) || opponentPiece(to, white);
}

bool Game::emptySpace(int r, int c) {
    return position_[r][c] == None;
}

bool Game::emptySpace(int pos[2]) {
    return emptySpace(pos[0], pos[1]);
}

bool Game::opponentPiece(int r, int c, bool white) {
    if (white)
        return position_[r][c] >= 6;
    else
        return position_[r][c] >= 0 && position_[r][c] <= 5;
}

bool Game::opponentPiece(int pos[2], bool white) {
    return opponentPiece(pos[0], pos[1], white);
}

bool Game::inCheck(bool white) {
    Piece king;
    if (white)
        king = WhiteKing;
    else
        king = BlackKing;

    bool found = false;
    int kingPos[2];
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (position_[r][c] == king) {
                kingPos[0] = r;
                kingPos[1] = c;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    int from[2];
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            from[0] = r;
            from[1] = c;
            if (validMove(from, kingPos)) {
                return true;
            }
        }
    }

    return false;
}

bool Game::inCheck(bool white, int from[2], int to[2]) {
    Piece boardCopy[8][8];
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            boardCopy[r][c] = position_[r][c];
        }
    }

    makeMove(from, to);

    bool check = inCheck(white);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            position_[r][c] = boardCopy[r][c];
        }
    }

    return check;
}

bool Game::isCheckMate() {
    if (!inCheck(whiteTurn_))
        return false;

    return !canMove();
}

bool Game::isDraw() {
    return isStalemate() || insufficientMaterial() || fiftyMoves() || isRepeat();
}

bool Game::isStalemate() {
    if (inCheck(whiteTurn_))
        return false;

    return !canMove();
}

bool Game::insufficientMaterial() {
    Piece piece;
    int pieceCount[12];
    for (int i = 0; i < 12; i++) {
        pieceCount[i] = 0;
    }

    bool whiteLightBishop = false;
    bool whiteDarkBishop = false;
    bool blackLightBishop = false;
    bool blackDarkBishop = false;
    bool lightSquare;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            piece = position_[r][c];
            if (piece == None)
                continue;
            pieceCount[piece]++;

            lightSquare = (r + c) % 2 == 0;
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

    int kingVsKing[12] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    int kingVsWhiteKnight[12] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    int kingVsWhiteBishop[12] = {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
    int kingVsBlackKnight[12] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1};
    int kingVsBlackBishop[12] = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1};
    int bishopsOnlyIndex[8] = {0, 1, 3, 4, 6, 7, 9, 10};
//    int bishopVsBishop[12] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};

    if (equalArrays(pieceCount, kingVsKing))
        return true;
    if (equalArrays(pieceCount, kingVsWhiteKnight))
        return true;
    if (equalArrays(pieceCount, kingVsWhiteBishop))
        return true;
    if (equalArrays(pieceCount, kingVsBlackKnight))
        return true;
    if (equalArrays(pieceCount, kingVsBlackBishop))
        return true;

    int ind;
    for (int i = 0; i < 8; i++) {
        ind = bishopsOnlyIndex[i];
        if (pieceCount[ind] != 0)
            return false;
    }

    // Only remaining pieces are kings and bishops

    if ((!whiteLightBishop) && (!blackLightBishop))
        return true;
    if ((!whiteDarkBishop) && (!blackDarkBishop))
        return true;

    return false;
}


bool Game::fiftyMoves() {
    if (movesNoProgess_ == 100)
        return true;
    return false;
}

bool Game::isRepeat() {
    Position currentPos = savePosition();

    int repLen = (int) repeatPositions_.size();
    int gameLen = (int) gameHistory_.size();

    for (int i = 0; i < repLen; i++) {
        if (currentPos == repeatPositions_[i]) {
            return true;
        }
    }

    for (int i = 1; i < gameLen; i++) {
        if (currentPos == gameHistory_[i]) {
            repeatPositions_.push_back(currentPos);
            break;
        }
    }

    gameHistory_.push_back(currentPos);
    return false;
}

bool Game::equalArrays(int a[12], int b[12]) {
    for (int i = 0; i < 12; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool Game::canMove() {
    Piece piece;
    int from[2];
    int to[2];
    for (int r_from = 0; r_from < 8; r_from++) {
        for (int c_from = 0; c_from < 8; c_from++) {
            piece = position_[r_from][c_from];
            if (whiteTurn_ && (piece < 0 || piece > 5))
                continue;
            if (!whiteTurn_ && (piece < 6))
                continue;

            from[0] = r_from;
            from[1] = c_from;
            for (int r_to = 0; r_to < 8; r_to++) {
                to[0] = r_to;
                for (int c_to = 0; c_to < 8; c_to++) {
                    to[1] = c_to;
                    if (validMove(from, to)) {
                        if (!inCheck(whiteTurn_, from, to))
                            return true;
                    }
                }
            }
        }
    }

    return false;
}

void Game::updateCastle(int from[2], int to[2]) {
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

void Game::updatePassant(int from[2], int to[2], Piece fromPiece) {
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

void Game::updateFiftyMoves(Piece fromPiece, Piece toPiece) {
    if (fromPiece == WhitePawn || fromPiece == BlackPawn) {
        movesNoProgess_ = 0;
        return;
    }
    if (toPiece != None) {
        movesNoProgess_ = 0;
        return;
    }

    movesNoProgess_++;
}



