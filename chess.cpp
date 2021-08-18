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
    : QMainWindow(parent), ui_(new Ui::Chess),
    game_(Game()),
    selected_{ -1, -1 },
    shownMoveNumber_(0),
    trueMoveNumber_(0),
    whiteTurn_(true)
{
    ui_->setupUi(this);

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

    layout_->add(leftLayout, ChessLayout::West);
    layout_->add(topLayout, ChessLayout::North);
    layout_->add(bottomLayout, ChessLayout::South);

    connect(newGame, &QPushButton::released, this, &Chess::newGame);

    Square *square;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            square = new Square(centralWidget, row, col);
            layout_->addSquare(square, row, col);

            connect(this, &Chess::setPiece, square, &Square::setPiece);
            connect(this, &Chess::highlightSquare, square, &Square::setHighlight);
            connect(square, &Square::clicked, this, &Chess::mousePress);
        }
    }

    whiteTimer_ = new Timer(this, true, 30000, 2000);
    blackTimer_ = new Timer(this, false, 30000, 5000);
    connect(this, &Chess::startTimer, whiteTimer_, &Timer::start);
    connect(this, &Chess::pauseTimer, whiteTimer_, &Timer::pause);
    connect(this, &Chess::resetTimer, whiteTimer_, &Timer::reset);
    connect(whiteTimer_, &Timer::currentTimeText, this, &Chess::updateTimerText);
    connect(whiteTimer_, &Timer::expiredTime, this, &Chess::expiredTime);

    connect(this, &Chess::startTimer, blackTimer_, &Timer::start);
    connect(this, &Chess::pauseTimer, blackTimer_, &Timer::pause);
    connect(this, &Chess::resetTimer, blackTimer_, &Timer::reset);
    connect(blackTimer_, &Timer::currentTimeText, this, &Chess::updateTimerText);
    connect(blackTimer_, &Timer::expiredTime, this, &Chess::expiredTime);

    whiteTimer_->updateText();
    blackTimer_->updateText();

    grabKeyboard();

    updatePosition();
}

Chess::~Chess()
{
    delete ui_;
    delete layout_;
    delete whiteTimer_;
    delete blackTimer_;
    delete topTimerLabel_;
    delete bottomTimerLabel_;
}

void Chess::newGame()
{
    game_.resetGame();
    shownMoveNumber_ = 0;
    trueMoveNumber_ = 0;
    whiteTurn_ = true;
    clearSelectedSquare();

    updatePosition();

    emit resetTimer(true, 300000, 0);
    emit resetTimer(false, 300000, 0);
}

void Chess::mousePress(int row, int col)
{
    if (shownMoveNumber_ != trueMoveNumber_)
        return;

    bool selectable = game_.isSelectable(row, col);

    if (selected_[0] == row && selected_[1] == col) {
        clearSelectedSquare();
    } else if (selectable) {
        setSelectedSquare(row, col);
    } else if (selected_[0] != -1) {
        int from[2] = {selected_[0], selected_[1]};
        int to[2] = {row, col};

        // TODO Replace int outcome with enumerated type for different outcomes
        int outcome = game_.tryMove(from, to);

        if (outcome == 1) {
            shownMoveNumber_++;
            trueMoveNumber_++;
            whiteTurn_ = !whiteTurn_;
            pressClock();
        } else if (outcome == 2)
            newGame();

        clearSelectedSquare();
        updatePosition();
    }
}

void Chess::updateTimerText(const QString &text, bool white)
{
    if (white)
        whiteTimerText_ = text;
    else
        blackTimerText_ = text;

    updateTimerLabels();
}

void Chess::expiredTime(bool white)
{
    newGame();
}

void Chess::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down) {
        if (shownMoveNumber_ == 0)
           return;
        shownMoveNumber_ = 0;
    } else if (event->key() == Qt::Key_Left) {
        if (shownMoveNumber_ == 0)
            return;
        shownMoveNumber_--;
    } else if (event->key() == Qt::Key_Right) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_++;
    } else if (event->key() == Qt::Key_Up) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_ = trueMoveNumber_;
    }

    clearSelectedSquare();
    updatePosition();
}

void Chess::updatePosition()
{
    Position position = game_.getPosition(shownMoveNumber_);

    Piece piece;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            piece = position.board[squareIndexAdjustment(row)]
                                  [squareIndexAdjustment(col)];
            emit setPiece(row, col, piece);
        }
    }
}

void Chess::setSelectedSquare(int row, int col)
{
    clearSelectedSquare();
    selected_[0] = row;
    selected_[1] = col;
    emit highlightSquare(row, col);
}

void Chess::clearSelectedSquare()
{
    if (selected_[0] == -1)
        return;

    emit highlightSquare(selected_[0], selected_[1]);
    selected_[0] = -1;
    selected_[1] = -1;
}

void Chess::pressClock()
{
    if (trueMoveNumber_ == 2)
        emit startTimer(whiteTurn_);
    else if (trueMoveNumber_ > 2) {
        emit startTimer(whiteTurn_);
        emit pauseTimer(!whiteTurn_);
    }
}

void Chess::updateTimerLabels()
{
    if (whiteTurn_) {
        bottomTimerLabel_->setText(whiteTimerText_);
        topTimerLabel_->setText(blackTimerText_);
    } else {
        bottomTimerLabel_->setText(blackTimerText_);
        topTimerLabel_->setText(whiteTimerText_);
    }
}

int Chess::squareIndexAdjustment(int rowOrColIndex) const
{
    if (whiteTurn_)
        return rowOrColIndex;
    else
        return 7 - rowOrColIndex;
}
