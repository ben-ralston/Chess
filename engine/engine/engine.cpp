#include "engine.h"

#include "node.h"

Engine::Engine() :
    QObject(),
    white_(false)
{
    Node::setMaxDepth(3);
    Node::setColor(white_);

    root_ = new Node();
}

void Engine::getMove(Move move)
{
    Node *newRoot;
    if (move != Move()) {
        newRoot = root_->getNewRoot(move);
        delete root_;
        root_ = newRoot;
    }

    Move bestMove = root_->getBestMove();

    newRoot = root_->getNewRoot(bestMove);
    delete root_;
    root_ = newRoot;

    emit chosenMove(bestMove);
}

void Engine::reset()
{
    Node::setColor(white_);

    delete root_;
    root_ = new Node();
}

void Engine::setColor(bool white)
{
    white_ = white;
}
