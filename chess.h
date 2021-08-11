#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include "piece.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Chess; }
QT_END_NAMESPACE

class Chess : public QMainWindow
{
    Q_OBJECT

public:
    Chess(QWidget *parent = nullptr);
    ~Chess();

public slots:
    void mousePress(int r, int c);
    void newGame();

signals:
    void setPiece(int r, int c, Piece p);
    void highlight(int r, int c);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Chess *ui;
    Piece curPosition[8][8];
    Game game;
    int selected[2];
    int moveNum;
    int trueMoveNum;
    bool whiteTurn;
    void updatePosition();
    void clearSelected();
    void setSelected(int r, int c);
};
#endif // CHESS_H
