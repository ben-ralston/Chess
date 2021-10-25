#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QFutureWatcher>

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

private slots:
    void getBestMoveFinished();

private:
    bool white_;
    Node *root_;
    QFutureWatcher<Move> watcher_;

};

#endif // ENGINE_H
