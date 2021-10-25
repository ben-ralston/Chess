#ifndef NODE_H
#define NODE_H

#include <vector>

#include "engine_game_state.h"
#include "rules/move.h"

class Node
{
public:
    Node();
    Node(Node *parent, const EngineGameState &gameState, Move move);
    ~Node();
    Node *getNewRoot(Move move);
    Move getBestMove();

    static void setColor(bool white);
    static void setMaxDepth(int newMaxDepth);

private:
    void generateChildren(int depth);
    void getGameHistory(std::vector<const Position> &history);  // Change vector type to const refs
    float getEvaluation();
    const Move &move() const;

    static bool playingAsWhite_;
    static int maxDepth_;

    const EngineGameState gameState_;
    const Position savedPosition_;  // TODO Replace with hash
    const Move move_;
    Node *parent_;
    std::vector<Node *> children_;
    float evaluation_;
    bool leaf_;
};

#endif // NODE_H
