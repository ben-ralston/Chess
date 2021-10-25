#include "engine.h"

Engine::Engine() :
    QObject(),
    white_(false)
{
}

void Engine::getMove(Move move)
{
    Move newMove;
    if (white_)
        newMove = Move(6, 4, 4, 4);
    else
        newMove = Move(1, 4, 3, 4);
    emit chosenMove(newMove);
}

void Engine::reset()
{

}

void Engine::setColor(bool white)
{
    white_ = white;
}
