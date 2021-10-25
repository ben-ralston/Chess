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
    float evaluatePosition(const std::vector<const Position> &gameHistory) const;

private:
//    const int rowValue = .03;

    int materialCount() const;
    float squareControl() const;
    float squareValue(int row, int col) const;

    float pawnStructure();

};

#endif // ENGINE_GAME_STATE_H
