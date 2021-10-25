#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

#include "rules/move.h"

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
};

#endif // ENGINE_H
