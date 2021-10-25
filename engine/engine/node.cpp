#include "node.h"

#include <vector>
#include <stdexcept>

#include "chess/piece.h"
#include "rules/move.h"

bool Node::playingAsWhite_;
int Node::maxDepth_;

Node::Node() :
    gameState_(),
    savedPosition_(gameState_.savePosition()),
    move_(),
    parent_(nullptr),
    leaf_(true)
{
    if (!playingAsWhite_) {
        generateChildren(1);
    }
}

Node::Node(Node *parent, const EngineGameState &gameState, Move move) :
    gameState_(gameState, move),
    savedPosition_(gameState_.savePosition()),
    move_(move),
    parent_(parent),
    leaf_(true)
{
}

Node::~Node()
{
    for (std::vector<Node *>::iterator it = children_.begin(); it != children_.end(); it++) {
        delete *it;
    }
}

Node *Node::getNewRoot(Move move)
{
    Node *newRoot = nullptr;
    for (std::vector<Node *>::iterator it = children_.begin(); it != children_.end(); it++) {
        if ((*it)->move() == move) {
            newRoot = *it;
            children_.erase(it);
            break;
        }
    }

    if (newRoot) {
        newRoot->parent_ = nullptr;
        return newRoot;
    } else
        throw std::runtime_error("Could not find move in Node::getNewRoot\n");
}

Move Node::getBestMove()
{
    generateChildren(maxDepth_);

    float bestEvaluation = children_[0]->getEvaluation();
    Move bestMove = children_[0]->move();

    float currentEvaluation;
    for (std::vector<Node *>::iterator it = ++children_.begin(); it != children_.end(); it++) {
        currentEvaluation = (*it)->getEvaluation();

        if (playingAsWhite_) {
            if (currentEvaluation > bestEvaluation) {
                bestEvaluation = currentEvaluation;
                bestMove = (*it)->move();
            }

        } else {
            if (currentEvaluation < bestEvaluation) {
                bestEvaluation = currentEvaluation;
                bestMove = (*it)->move();
            }
        }
    }

    return bestMove;
}

void Node::setColor(bool white)
{
    playingAsWhite_ = white;
}

void Node::setMaxDepth(int newMaxDepth)
{
    maxDepth_ = newMaxDepth;
}

void Node::generateChildren(int depth)
{
    // TODO Check whether game is over before generating children
    if (!leaf_) {
        if (depth > 1) {
            for (std::vector<Node *>::iterator it = children_.begin(); it != children_.end(); it++) {
                (*it)->generateChildren(depth - 1);
            }
        }

        return;
    }

    std::vector<Move> legalMoves;
    std::vector<Move> promotionMoves;
    gameState_.getLegalMoves(legalMoves, promotionMoves);

    if (legalMoves.size() == 0 && promotionMoves.size() == 0)
        return;

    leaf_ = false;

    int from[2];
    int to[2];
    for (std::vector<const Move>::iterator it = legalMoves.begin(); it != legalMoves.end(); it++) {
        it->getFromTo(from, to);

        children_.push_back(new Node(this, gameState_, Move(from, to)));
    }

    for (std::vector<const Move>::iterator it = promotionMoves.begin(); it != promotionMoves.end(); it++) {
        it->getFromTo(from, to);

        Piece pieces[4];
        if (gameState_.whiteTurn()) {
            pieces[0] = WhiteKnight;
            pieces[1] = WhiteBishop;
            pieces[2] = WhiteRook;
            pieces[3] = WhiteQueen;
        } else {
            pieces[0] = BlackKnight;
            pieces[1] = BlackBishop;
            pieces[2] = BlackRook;
            pieces[3] = BlackQueen;
        }

        for (int i = 0; i < 4; i++) {
            children_.push_back(new Node(this, gameState_, Move(from, to, pieces[i])));
        }
    }

    if (depth > 1) {
        for (std::vector<Node *>::iterator it = children_.begin(); it != children_.end(); it++) {
            (*it)->generateChildren(depth - 1);
        }
    }
}

void Node::getGameHistory(std::vector<const Position> &history)
{
    if (parent_ == nullptr)
        history.push_back(savedPosition_);
    else {
        parent_->getGameHistory(history);
        history.push_back(savedPosition_);
    }
}

float Node::getEvaluation()
{
    if (leaf_) {
        std::vector<const Position> history;
        getGameHistory(history);
        evaluation_ = gameState_.evaluatePosition(history);
        return evaluation_;
    }

    bool white = gameState_.whiteTurn();

    float current;
    float extreme = children_[0]->getEvaluation();
    for (std::vector<Node *>::iterator it = ++children_.begin(); it != children_.end(); it++) {
        current = (*it)->getEvaluation();

        if (white) {
            if (current > extreme)
                extreme = current;
        } else {
            if (current < extreme)
                extreme = current;
        }

    }

    evaluation_ = extreme;
    return extreme;
}

const Move &Node::move() const
{
    return move_;
}
