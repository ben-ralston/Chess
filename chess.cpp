#include <iostream>

#include "chess.h"
#include "ui_chess.h"
#include "square.h"

using namespace std;

Chess::Chess(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::Chess)
{
    ui->setupUi(this);
    QWidget *centralW = this->findChild<QWidget *>("centralwidget");
    QPushButton *newGame = this->findChild<QPushButton *>("newGame");
    topTimerLabel = this->findChild<QLabel *>("topTimerLabel");
    bottomTimerLabel = this->findChild<QLabel *>("bottomTimerLabel");

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(newGame, 0, Qt::AlignTop);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(topTimerLabel, 0, Qt::AlignLeft);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(bottomTimerLabel, 0, Qt::AlignRight);

    grabKeyboard();

    game = Game();

    selected[0] = -1;
    selected[1] = -1;

    Square *square;

    layout = new ChessLayout(centralW, QMargins(5, 5, 5, 5), 5);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            square = new Square(centralW, row, col);
            layout->addSquare(square, row, col);

            connect(square, &Square::clicked, this, &Chess::mousePress);
            connect(this, &Chess::setPiece, square, &Square::setPiece);
            connect(this, &Chess::highlight, square, &Square::setHighlight);
        }
    }

    moveNum = 0;
    trueMoveNum = 0;
    whiteTurn = true;
    updatePosition();

    layout->add(leftLayout, ChessLayout::West);
    layout->add(topLayout, ChessLayout::North);
    layout->add(bottomLayout, ChessLayout::South);

    connect(newGame, &QPushButton::released, this, &Chess::newGame);

    whiteTimer = new Timer(this, 30000, 2000, true);
    blackTimer = new Timer(this, 30000, 5000, false);
//    whiteTimer = new Timer(this, 300000, 0, true);
//    blackTimer = new Timer(this, 300000, 0, false);
    connect(this, &Chess::startTimer, whiteTimer, &Timer::start);
    connect(this, &Chess::pauseTimer, whiteTimer, &Timer::pause);
    connect(this, &Chess::resetTimer, whiteTimer, &Timer::reset);
    connect(whiteTimer, &Timer::changeText, this, &Chess::updateText);
    connect(whiteTimer, &Timer::expiredTime, this, &Chess::expiredTime);

    connect(this, &Chess::startTimer, blackTimer, &Timer::start);
    connect(this, &Chess::pauseTimer, blackTimer, &Timer::pause);
    connect(this, &Chess::resetTimer, blackTimer, &Timer::reset);
    connect(blackTimer, &Timer::changeText, this, &Chess::updateText);
    connect(blackTimer, &Timer::expiredTime, this, &Chess::expiredTime);

    whiteTimer->setText();
    blackTimer->setText();
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
            pressClock();
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

    emit resetTimer(300000, 0, true);
    emit resetTimer(300000, 0, false);
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

void Chess::updateText(QString text, bool white) {
    if (white)
        whiteTime = text;
    else
        blackTime = text;

    updateLabels();
}

void Chess::expiredTime(bool white) {
    newGame();
}

void Chess::pressClock() {
    if (trueMoveNum > 1) {
        emit startTimer(whiteTurn);
        emit pauseTimer(!whiteTurn);
    }
}

void Chess::updateLabels() {
    if (whiteTurn) {
        bottomTimerLabel->setText(whiteTime);
        topTimerLabel->setText(blackTime);
    } else {
        bottomTimerLabel->setText(blackTime);
        topTimerLabel->setText(whiteTime);
    }
}
