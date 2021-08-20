#include "chess.h"

#include <QKeyEvent>

#include "ui_chess.h"
#include "game.h"
#include "chess_layout.h"
#include "square.h"
#include "timer.h"
#include "position.h"
#include "piece.h"

using namespace std;

Chess::Chess(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::Chess)
{
    ui_->setupUi(this);

    game_ = new Game(this);
    connect(this, &Chess::keyPress, game_, &Game::keyPress);

    QWidget *centralWidget = this->findChild<QWidget *>("centralwidget");
    QPushButton *newGame = this->findChild<QPushButton *>("newGame");
    topTimerLabel_ = this->findChild<QLabel *>("topTimerLabel");
    bottomTimerLabel_ = this->findChild<QLabel *>("bottomTimerLabel");

    layout_ = new ChessLayout(centralWidget, QMargins(5, 5, 5, 5), 5);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    leftLayout->addWidget(newGame, 0, Qt::AlignTop);
    topLayout->addWidget(topTimerLabel_, 0, Qt::AlignLeft);
    bottomLayout->addWidget(bottomTimerLabel_, 0, Qt::AlignRight);

    layout_->addLayout(leftLayout, ChessLayout::West);
    layout_->addLayout(topLayout, ChessLayout::North);
    layout_->addLayout(bottomLayout, ChessLayout::South);

    QWidget *east;
    east = new QWidget(this);
    east->setStyleSheet("background-color:blue;");
    east->show();
    layout_->addWidget(east, ChessLayout::East);

    connect(newGame, &QPushButton::released, this, &Chess::newGame);

    Square *square;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            square = new Square(centralWidget, row, col);
            layout_->addWidget(square, ChessLayout::Board, row, col);

            connect(game_, &Game::setPiece, square, &Square::setPiece);
            connect(game_, &Game::highlightSquare, square, &Square::setHighlight);
            connect(square, &Square::clicked, game_, &Game::mousePress);
        }
    }

    whiteTimer_ = new Timer(this, true, 30000, 2000);
    blackTimer_ = new Timer(this, false, 30000, 5000);
    connect(this, &Chess::startTimer, whiteTimer_, &Timer::start);
    connect(this, &Chess::pauseTimer, whiteTimer_, &Timer::pause);
    connect(this, &Chess::resetTimer, whiteTimer_, &Timer::reset);
    connect(whiteTimer_, &Timer::currentTimeText, this, &Chess::updateTimerText);
    connect(whiteTimer_, &Timer::expiredTime, game_, &Game::expiredTime);

    connect(this, &Chess::startTimer, blackTimer_, &Timer::start);
    connect(this, &Chess::pauseTimer, blackTimer_, &Timer::pause);
    connect(this, &Chess::resetTimer, blackTimer_, &Timer::reset);
    connect(blackTimer_, &Timer::currentTimeText, this, &Chess::updateTimerText);
    connect(blackTimer_, &Timer::expiredTime, game_, &Game::expiredTime);

    whiteTimer_->updateText();
    blackTimer_->updateText();

    grabKeyboard();

    setMinimumSize(layout_->minimumSize());

    game_->updatePosition();
}

Chess::~Chess()
{
    delete ui_;
    delete game_;
    delete layout_;
    delete whiteTimer_;
    delete blackTimer_;
    delete topTimerLabel_;
    delete bottomTimerLabel_;
}

void Chess::newGame()
{
    game_->resetGame();

    emit resetTimer(true, 300000, 0);
    emit resetTimer(false, 300000, 0);
}

void Chess::updateTimerText(const QString &text, bool white)
{
    if (white)
        whiteTimerText_ = text;
    else
        blackTimerText_ = text;

    updateTimerLabels();
}


// TODO Transfer keypress control to Game
void Chess::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Left)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Right)
        emit keyPress(event->key());
    else if (event->key() == Qt::Key_Up)
        emit keyPress(event->key());
}

void Chess::pressClock()
{
//    if (trueMoveNumber_ == 2)
//        emit startTimer(whiteTurn_);
//    else if (trueMoveNumber_ > 2) {
//        emit startTimer(whiteTurn_);
//        emit pauseTimer(!whiteTurn_);
//    }
}

void Chess::updateTimerLabels()
{
//    if (whiteTurn_) {
//        bottomTimerLabel_->setText(whiteTimerText_);
//        topTimerLabel_->setText(blackTimerText_);
//    } else {
//        bottomTimerLabel_->setText(blackTimerText_);
//        topTimerLabel_->setText(whiteTimerText_);
//    }
}
