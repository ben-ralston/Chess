#include "engine_game_state.h"

#include <algorithm>

#include "chess/piece.h"
#include "rules/game_state.h"

EngineGameState::EngineGameState() : GameState()
{
}

EngineGameState::EngineGameState(const GameState &gameState, Move move) :
    GameState(gameState)
{
    int from[2];
    int to[2];
    move.getFromTo(from, to);

    makeMoveAndUpdate(from, to, move.promotionPiece);
}

EngineGameState::EngineGameState(const GameState &gameState, int from[2], int to[2], Piece promotionPiece) :
    GameState(gameState)
{
    makeMoveAndUpdate(from, to, promotionPiece);
}

float EngineGameState::evaluatePosition(const std::vector<const Position> &gameHistory) const
{
    if (gameHistory.size() > 0) {
        VictoryType result = getOutcome(gameHistory);
        if (result == WhiteCheckmate)
            return 1000;
        if (result == BlackCheckmate)
            return -1000;
        if (result != NA)
            return 0;
    }

    float evaluation = 0;

    evaluation += static_cast<float>(materialCount());
    evaluation += squareControl();

    return evaluation;
}

bool EngineGameState::whiteTurn() const
{
    return whiteTurn_;
}

int EngineGameState::materialCount() const
{
    int count = 0;

    Piece piece;
    int pos[2];
    for (pos[0] = 0; pos[0] < 8; pos[0]++) {
        for (pos[1] = 0; pos[1] < 8; pos[1]++) {
            piece = pieceAt(pos, board_);

            switch (piece)
            {
                case None:
                    break;
                case WhitePawn:
                    count++;
                    break;
                case BlackPawn:
                    count--;
                    break;
                case WhiteKnight:
                    count += 3;
                    break;
                case BlackKnight:
                    count -= 3;
                    break;
                case WhiteBishop:
                    count += 3;
                    break;
                case BlackBishop:
                    count -= 3;
                    break;
                case WhiteRook:
                    count += 5;
                    break;
                case BlackRook:
                    count -= 5;
                    break;
                case WhiteQueen:
                    count += 9;
                    break;
                case BlackQueen:
                    count -= 9;
                    break;
                case WhiteKing:
                    break;
                case BlackKing:
                    break;
            }
        }
    }

    return count;
}

float EngineGameState::squareControl() const
{
    float value = 0;

    int from[2];
    int to[2];
    bool whiteTurn;

    for (from[0] = 0; from[0] < 8; from[0]++) {
        for (from[1] = 0; from[1] < 8; from[1]++) {
            if (pieceAt(from, board_) == None)
                continue;

            if (pieceAt(from, board_) == WhitePawn) {
                if (from[1] == 0)
                    value += 2 * squareValue(from[0] - 1, 1);
                else if (from[1] == 7)
                    value += 2 * squareValue(from[0] - 1, 6);
                else {
                    value += 2 * squareValue(from[0] - 1, from[1] + 1);
                    value += 2 * squareValue(from[0] - 1, from[1] - 1);
                }

                continue;
            }

            if (pieceAt(from, board_) == BlackPawn) {
                if (from[1] == 0)
                    value -= 2 * squareValue(from[0] + 1, 1);
                else if (from[1] == 7)
                    value -= 2 * squareValue(from[0] + 1, 6);
                else {
                    value -= 2 * squareValue(from[0] + 1, from[1] + 1);
                    value -= 2 * squareValue(from[0] + 1, from[1] - 1);
                }

                continue;
            }

            whiteTurn = opponentPiece(from, false, board_);

            for (to[0] = 0; to[0] < 8; to[0]++) {
                for (to[1] = 0; to[1] < 8; to[1]++) {
                    if (legalMove(from, to, whiteTurn, true))
                        value += (whiteTurn ? 1 : -1) * squareValue(to[0], to[1]);
                }
            }
        }
    }

    return value;
}

float EngineGameState::squareValue(int row, int col) const
{
    float value = .03;

    int rowOrder;
    int colOrder;

    if (row < 4)
        rowOrder = row + 1;
    else
        rowOrder = 8 - row;

    if (col < 4)
        colOrder = col + 1;
    else
        colOrder = 8 - col;

    return std::min(rowOrder, colOrder) * value;
}

float EngineGameState::pawnStructure()
{
    // TODO Add implementation calculating pawn structure strength
    // TODO Factors: Connected pawns/pawn chains, *passed pawns*, doubled pawns, isolated pawns

    return 0;
}

