#include "engine_game_state.h"

#include "chess/piece.h"
#include "rules/game_state.h"

EngineGameState::EngineGameState() : GameState()
{
}

//EngineGameState::EngineGameState(const EngineGameState &gameState) : GameState(gameState)
//{
//}

EngineGameState::EngineGameState(const GameState &gameState, int from[2], int to[2], Piece promotionPiece) :
    GameState(gameState)
{
    makeMoveAndUpdate(from, to, promotionPiece);
}

float EngineGameState::evaluatePosition()
{
    float evaluation = 0;

    evaluation += static_cast<float>(materialCount());

    return evaluation;
}

int EngineGameState::materialCount()
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

