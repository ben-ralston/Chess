#include "game.h"

using namespace std;

Game::Game()
{
    whiteTurn = true;
    whiteKingsideCastle = true;
    whiteQueensideCastle = true;
    blackKingsideCastle = true;
    blackQueensideCastle = true;
    whitePassantPawn = -1;
    blackPassantPawn = -1;
    setStartingPosition();
}

void Game::setStartingPosition() {
    for (int r = 0; r < 8; r++) {
        if (r == 0) {
            array<Piece,8> row = {BlackRook, BlackKnight, BlackBishop, BlackQueen,
                                  BlackKing, BlackBishop, BlackKnight, BlackRook};
            for (int c = 0; c < 8; c++) {
                position[r][c] = row[c];
            }
        } else if (r == 1) {
            array<Piece,8> row = {BlackPawn, BlackPawn, BlackPawn, BlackPawn,
                                      BlackPawn, BlackPawn, BlackPawn, BlackPawn};
            for (int c = 0; c < 8; c++) {
                position[r][c] = row[c];
            }
        } else if (r == 6) {
            array<Piece,8> row = {WhitePawn, WhitePawn, WhitePawn, WhitePawn,
                                      WhitePawn, WhitePawn, WhitePawn, WhitePawn};
            for (int c = 0; c < 8; c++) {
                position[r][c] = row[c];
            }
        } else if (r == 7) {
            array<Piece,8> row = {WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen,
                                      WhiteKing, WhiteBishop, WhiteKnight, WhiteRook};
            for (int c = 0; c < 8; c++) {
                position[r][c] = row[c];
            }
        } else {
            array<Piece,8> row = {None, None, None, None,
                                  None, None, None, None};
            for (int c = 0; c < 8; c++) {
                position[r][c] = row[c];
            }
        }
    }
}

Piece * Game::getPosition() {
    Piece * ptr = &position[0][0];
    return ptr;
}

bool Game::isSelectable(int row, int col) {
    if (whiteTurn) {
        if (position[row][col] >= 0 && position[row][col] <= 5)
            return true;
        else
            return false;
    } else {
        if (position[row][col] >= 6)
            return true;
        else
            return false;
    }
}

void Game::tryMove(int from[2], int to[2]) {
    if (!validMove(from, to))
        return;
    if (inCheck(whiteTurn, from, to))
        return;

    Piece fromPiece = position[from[0]][from[1]];
    makeMove(from, to);
    updateCastle(from, to);
    updatePassant(from, to, fromPiece);
    whiteTurn = !whiteTurn;

    if (isCheckMate()) {
        setStartingPosition();
        whiteTurn = true;
//        return;
    }

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
    Piece toMove = position[from[0]][from[1]];
    position[from[0]][from[1]] = None;
    position[to[0]][to[1]] = toMove;
}

bool Game::makeCastleMove(int from[2], int to[2]) {

    if (position[from[0]][from[1]] == WhiteKing) {
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

    if (position[from[0]][from[1]] == BlackKing) {
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

    if (position[from[0]][from[1]] == WhitePawn) {
        removeDirection = 1;
        passantRow = 2;
        passantCol = blackPassantPawn;

    } else if (position[from[0]][from[1]] == BlackPawn) {
        removeDirection = -1;
        passantRow = 5;
        passantCol = whitePassantPawn;
    } else
        return false;

    if (to[0] == passantRow && to[1] == passantCol) {
        makeStandardMove(from, to);
        position[passantRow + removeDirection][passantCol] = None;
        return true;
    }

    return false;
}

bool Game::makePromotionMove(int from[2], int to[2], bool autoQueen) {
    int promRow;
    Piece promPiece;

    if (position[from[0]][from[1]] == WhitePawn) {
        promRow = 0;
        promPiece = WhiteQueen;
    } else if (position[from[0]][from[1]] == BlackPawn) {
        promRow = 7;
        promPiece = BlackQueen;
    } else
        return false;

    if (to[0] == promRow) {
        position[from[0]][from[1]] = None;
        position[to[0]][to[1]] = promPiece;
        return true;
    }

    return false;
}

bool Game::validMove(int from[2], int to[2]) {
    switch (position[from[0]][from[1]]) {
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
        passantCol = blackPassantPawn;
    }
    else {
        direction = 1;
        firstMove = from[0] == 1;
        passantRow = 5;
        passantCol = whitePassantPawn;
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
            if (!whiteKingsideCastle)
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
            if (!whiteQueensideCastle)
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
            if (!blackKingsideCastle)
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
            if (!blackQueensideCastle)
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
    return position[r][c] == None;
}

bool Game::emptySpace(int pos[2]) {
    return emptySpace(pos[0], pos[1]);
}

bool Game::opponentPiece(int r, int c, bool white) {
    if (white)
        return position[r][c] >= 6;
    else
        return position[r][c] >= 0 && position[r][c] <= 5;
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
            if (position[r][c] == king) {
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
            boardCopy[r][c] = position[r][c];
        }
    }

    makeMove(from, to);

    bool check = inCheck(white);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            position[r][c] = boardCopy[r][c];
        }
    }

    return check;
}

bool Game::isCheckMate() {
    if (!inCheck(whiteTurn))
        return false;

    Piece piece;
    int from[2];
    int to[2];
    for (int r_from = 0; r_from < 8; r_from++) {
        for (int c_from = 0; c_from < 8; c_from++) {
            piece = position[r_from][c_from];
            if (whiteTurn && (piece < 0 || piece > 5))
                continue;
            if (!whiteTurn && (piece < 6))
                continue;

            from[0] = r_from;
            from[1] = c_from;
            for (int r_to = 0; r_to < 8; r_to++) {
                to[0] = r_to;
                for (int c_to = 0; c_to < 8; c_to++) {
                    to[1] = c_to;
                    if (validMove(from, to)) {
                        if (!inCheck(whiteTurn, from, to))
                            return false;
                    }
                }
            }
        }
    }

    return true;
}

void Game::updateCastle(int from[2], int to[2]) {
    if (from[0] == 7 && from[1] == 4) {
        whiteKingsideCastle = false;
        whiteQueensideCastle = false;
    }
    if (from[0] == 0 && from[1] == 4) {
        blackKingsideCastle = false;
        blackQueensideCastle = false;
    }
    if ((from[0] == 7 && from[1] == 7) || (to[0] == 7 && to[1] == 7)) {
        whiteKingsideCastle = false;
    }
    if ((from[0] == 7 && from[1] == 0) || (to[0] == 7 && to[1] == 0)) {
        whiteQueensideCastle = false;
    }
    if ((from[0] == 0 && from[1] == 7) || (to[0] == 0 && to[1] == 7)) {
        blackKingsideCastle = false;
    }
    if ((from[0] == 0 && from[1] == 0) || (to[0] == 0 && to[1] == 0)) {
        blackQueensideCastle = false;
    }
}

void Game::updatePassant(int from[2], int to[2], Piece fromPiece) {
    if (fromPiece == WhitePawn) {
        if (to[0] - from[0] == -2) {
            whitePassantPawn = to[1];
            blackPassantPawn = -1;
            return;
        }
    } else if (fromPiece == BlackPawn) {
        if (to[0] - from[0] == 2) {
            whitePassantPawn = -1;
            blackPassantPawn = to[1];
            return;
        }
    }

    whitePassantPawn = -1;
    blackPassantPawn = -1;
}





