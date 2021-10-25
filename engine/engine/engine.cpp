#include "engine.h"

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include "node.h"

Engine::Engine() :
    QObject(),
    white_(false)
{
    connect(&watcher_, &QFutureWatcher<Move>::finished, this, &Engine::getBestMoveFinished);

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

    QFuture<Move> future = QtConcurrent::run(&Node::getBestMove, root_);
    watcher_.setFuture(future);
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

void Engine::getBestMoveFinished()
{
    Move bestMove = watcher_.future().result();

    Node *newRoot = root_->getNewRoot(bestMove);
    delete root_;
    root_ = newRoot;

    emit chosenMove(bestMove);
}
