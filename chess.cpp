#include <iostream>

#include "chess.h"
#include "ui_chess.h"
#include "square.h"

using namespace std;

Chess::Chess(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::Chess)
{
    ui->setupUi(this);

    QPushButton *newGame = this->findChild<QPushButton *>("newGame");
    delete newGame;
    QWidget *board = this->findChild<QWidget *>("board");
    delete board;

    QWidget *centralW = this->findChild<QWidget *>("centralwidget");

    layout = new ChessLayout(centralW, QMargins(0, 0, 0, 0), 0);
//    layout->show()

//    grabKeyboard();

//    game = Game();

//    selected[0] = -1;
//    selected[1] = -1;

//    QPushButton *newGame = this->findChild<QPushButton *>("newGame");
//    QWidget *board = this->findChild<QWidget *>("board");
//    Square *square;

//    for (int row = 0; row < 8; row++) {
//        for (int col = 0; col < 8; col++) {
//            square = new Square(board, row, col);
//            connect(square, &Square::clicked, this, &Chess::mousePress);
//            connect(this, &Chess::setPiece, square, &Square::setPiece);
//            connect(this, &Chess::highlight, square, &Square::setHighlight);
//        }
//    }

//    moveNum = 0;
//    trueMoveNum = 0;
//    whiteTurn = true;
//    updatePosition();

//    connect(newGame, &QPushButton::released, this, &Chess::newGame);
}

Chess::~Chess()
{
    delete ui;
}

void Chess::mousePress(int r, int c) {
    if (moveNum != trueMoveNum)
        return;

    bool selectable = game.isSelectable(r, c);

    if (selected[0] == -1 && selectable) {
        setSelected(r, c);
    } else if (selected[0] == r && selected[1] == c) {
        clearSelected();
    } else if (selectable) {
        setSelected(r, c);
    } else if (selected[0] != -1) {
        int from[2] = {selected[0], selected[1]};
        int to[2] = {r, c};
        int outcome = game.tryMove(from, to);
        if (outcome == 1) {
            moveNum++;
            trueMoveNum++;
            whiteTurn = !whiteTurn;
        } else if (outcome == 2) {
            newGame();
            return;
        }

        clearSelected();
        updatePosition();
    }
}

void Chess::updatePosition() {
    Position pos = game.getPosition(moveNum);

    if (whiteTurn) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
//                cout << pos.board[r][c] << '\n';
                emit setPiece(r, c, pos.board[r][c]);
            }
        }
    } else {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                emit setPiece(r, c, pos.board[7 - r][7 - c]);
            }
        }
    }
}

void Chess::clearSelected() {
    if (selected[0] == -1) return;
    emit highlight(selected[0], selected[1]);
    selected[0] = -1;
    selected[1] = -1;
}

void Chess::setSelected(int r, int c) {
    clearSelected();
    selected[0] = r;
    selected[1] = c;
    emit highlight(r, c);
}

void Chess::newGame() {
    moveNum = 0;
    trueMoveNum = 0;
    whiteTurn = true;
    clearSelected();

    game.resetGame();
    updatePosition();
}

void Chess::keyPressEvent(QKeyEvent *event) {
    // Only clear selected if key press does something

    if (event->key() == Qt::Key_Left)
    {
        if (moveNum == 0)
            return;
        moveNum--;
        clearSelected();
        updatePosition();
    } else if (event->key() == Qt::Key_Right) {
        if (moveNum == trueMoveNum)
            return;
        moveNum++;
        clearSelected();
        updatePosition();
    } else if (event->key() == Qt::Key_Up) {
        moveNum = trueMoveNum;
        clearSelected();
        updatePosition();
    } else if (event->key() == Qt::Key_Down) {
        moveNum = 0;
        clearSelected();
        updatePosition();
    }
}
