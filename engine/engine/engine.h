#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

#include "rules/move.h"
#include "node.h"

class Engine : public QObject
{
    Q_OBJECT

public:
    Engine();
    void getMove(Move move);
    void reset();
    void setColor(bool white);

signals:
    void chosenMove(Move move);

private:
    bool white_;
    Node *root_;
};

#endif // ENGINE_H
