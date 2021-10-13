#ifndef ENGINE_GAME_STATE_H
#define ENGINE_GAME_STATE_H

#include "chess/piece.h"
#include "rules/game_state.h"

class EngineGameState : public GameState
{
public:
    EngineGameState();
//    EngineGameState(const EngineGameState &gameState);
    EngineGameState(const GameState &gameState, int from[2], int to[2], Piece promotionPiece);
    float evaluatePosition();

private:
    int materialCount();

};

#endif // ENGINE_GAME_STATE_H
