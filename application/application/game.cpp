#include "game.h"

#include <array>
#include <algorithm>
#include <iostream>

#include <QKeyEvent>

#include "application_game_state.h"
#include "engine/engine_game_state.h"
#include "chess/piece.h"
#include "rules/game_state.h"
#include "rules/position.h"
#include "rules/move.h"

using namespace std;

Game::Game(QObject *parent, int whiteTime, int blackTime, int whiteIncrement, int blackIncrement) :
    QObject(parent),
    selectedSquare_{-1, -1},
    flipBoard_(true),
    twoPlayer_(true),
    whiteVsComputer_(true)
{
    whiteTimer_ = new Timer(this, true);
    blackTimer_ = new Timer(this, false);

    connect(this, &Game::startTimer, whiteTimer_, &Timer::start);
    connect(this, &Game::pauseTimer, whiteTimer_, &Timer::pause);
    connect(this, &Game::resetTimer, whiteTimer_, &Timer::reset);
    connect(whiteTimer_, &Timer::currentTimeText, this, &Game::updateTimerText);
    connect(whiteTimer_, &Timer::expiredTime, this, &Game::expiredTime);

    connect(this, &Game::startTimer, blackTimer_, &Timer::start);
    connect(this, &Game::pauseTimer, blackTimer_, &Timer::pause);
    connect(this, &Game::resetTimer, blackTimer_, &Timer::reset);
    connect(blackTimer_, &Timer::currentTimeText, this, &Game::updateTimerText);
    connect(blackTimer_, &Timer::expiredTime, this, &Game::expiredTime);

    setTimeControl(whiteTime, blackTime, whiteIncrement, blackIncrement);

    connect(&engine_, &Engine::chosenMove, this, &Game::engineMove);

    reset();
}

Game::~Game()
{
    delete whiteTimer_;
    delete blackTimer_;
}

void Game::updateGame()
{
    updatePosition();
    updateClocks();
}

void Game::setTimeControl(int whiteTime, int blackTime, int whiteIncrement, int blackIncrement)
{
    whiteTime_ = whiteTime;
    blackTime_ = blackTime;
    whiteIncrement_ = whiteIncrement;
    blackIncrement_ = blackIncrement;
}

void Game::setFlipBoard(bool newFlipBoard)
{
    flipBoard_ = newFlipBoard;
    if (!whiteTurn_)
        rotateSelectedSquare();
}

void Game::setWhiteVsComputer(bool newWhiteVsComputer)
{
    whiteVsComputer_ = newWhiteVsComputer;
}

void Game::setTwoPlayer(bool newTwoPlayer)
{
    twoPlayer_ = newTwoPlayer;
}

void Game::mousePress(int row, int col)
{
    if (gameOver_)
        return;
    if (shownMoveNumber_ != trueMoveNumber_)
        return;

    if (!twoPlayer_) {
        if (trueMoveNumber_ % 2 == whiteVsComputer_)
            return;
    }

    if (choosingPromotionPiece_) {
        choosingPromotionPiece_ = false;
        emit setPromotionVisibilty(false);
        clearSelectedSquare();
        return;
    }

    int adjRow = indexAdjustment(row);
    int adjCol = indexAdjustment(col);
    bool selectable = gameState_.isSelectable(adjRow, adjCol);

    if (selectedSquare_[0] == row && selectedSquare_[1] == col) {
        clearSelectedSquare();
        return;
    }
    if (selectable) {
        setSelectedSquare(row, col);
        return;
    }
    if (selectedSquare_[0] == -1)
        return;

    int from[2] = { indexAdjustment(selectedSquare_[0]), indexAdjustment(selectedSquare_[1]) };
    int to[2] = {adjRow, adjCol};

    if (vectorContains(from, to, promotionMoves_)) {
        promotionFrom_[0] = from[0];
        promotionFrom_[1] = from[1];
        promotionTo_[0] = to[0];
        promotionTo_[1] = to[1];

        choosingPromotionPiece_ = true;

        emit setPromotionColor(whiteTurn_);
        emit setPromotionVisibilty(true);
    } else if (vectorContains(from, to, legalMoves_)){
        QString notation = gameState_.move(from, to, None);
        emit notateMove(notation);

        processMove(Move(from, to));
    } else
        clearSelectedSquare();
}

void Game::keyPress(int key)
{
    if (choosingPromotionPiece_)
        return;

    if (key == Qt::Key_Down) {
        if (shownMoveNumber_ == 0)
           return;
        shownMoveNumber_ = 0;
    } else if (key == Qt::Key_Left) {
        if (shownMoveNumber_ == 0)
            return;
        shownMoveNumber_--;
    } else if (key == Qt::Key_Right) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_++;
    } else if (key == Qt::Key_Up) {
        if (shownMoveNumber_ == trueMoveNumber_)
            return;
        shownMoveNumber_ = trueMoveNumber_;
    }

    emit notationMoveNumber(shownMoveNumber_ - 1);
    clearSelectedSquare();
    updatePosition();
}

void Game::completePromotion(Piece piece)
{
    emit setPromotionVisibilty(false);
    choosingPromotionPiece_ = false;

    QString notation = gameState_.move(promotionFrom_, promotionTo_, piece);
    emit notateMove(notation);

    processMove(Move(promotionFrom_, promotionTo_, piece));
}

void Game::expiredTime(bool white)
{
    QString color = white ? "Black Wins!" : "White Wins!";
    QString type = "on Time";
    emit gameEnded(color, type);

    emit pauseTimer(whiteTurn_, true);
    clearSelectedSquare();
    updatePosition();
    gameOver_ = true;
}

void Game::updateShownMove(int move)
{
    if (choosingPromotionPiece_) {
        choosingPromotionPiece_ = false;
        emit setPromotionVisibilty(false);
    }

    shownMoveNumber_ = move;
    clearSelectedSquare();
    updatePosition();
}

void Game::updateTimerText(const QString &text, bool white)
{
    if (white)
        whiteTimerText_ = text;
    else
        blackTimerText_ = text;

    updateClocks();
}

void Game::reset()
{
    gameOver_ = false;
    whiteTurn_ = true;
    clearSelectedSquare();
    shownMoveNumber_ = 0;
    trueMoveNumber_ = 0;
    choosingPromotionPiece_ = false;

    gameState_.reset();
    gameState_.getLegalMoves(legalMoves_, promotionMoves_);

    engine_.setColor(!whiteVsComputer_);
    engine_.reset();

    gameHistory_.clear();
    gameHistory_.push_back(gameState_.savePosition());

    emit resetTimer(true, whiteTime_, whiteIncrement_);
    emit resetTimer(false, blackTime_, blackIncrement_);
    emit clearNotation();
    emit notationMoveNumber(-1);
    emit setPromotionVisibilty(false);

    updatePosition();
    updateClocks();

    if (!twoPlayer_ && !whiteVsComputer_)
        engine_.getMove(Move());
}

void Game::engineMove(Move move)
{
    int from[2];
    int to[2];
    move.getFromTo(from, to);

    QString notation = gameState_.move(from, to, move.promotionPiece);
    emit notateMove(notation);

    processMove(move);
}

void Game::setSelectedSquare(int row, int col)
{
    clearSelectedSquare();
    selectedSquare_[0] = row;
    selectedSquare_[1] = col;
    emit highlightSquare(selectedSquare_);
}

void Game::clearSelectedSquare()
{
    if (selectedSquare_[0] == -1)
        return;

    emit highlightSquare(selectedSquare_);
    selectedSquare_[0] = -1;
    selectedSquare_[1] = -1;
}

void Game::rotateSelectedSquare()
{
    if (selectedSquare_[0] == -1)
        return;
    setSelectedSquare(7 - selectedSquare_[0], 7 - selectedSquare_[1]);
}

void Game::processMove(Move move)
{
    gameHistory_.push_back(gameState_.savePosition());
    GameState::VictoryType victory = gameState_.getOutcome(gameHistory_);

    shownMoveNumber_++;
    trueMoveNumber_++;
    emit notationMoveNumber(shownMoveNumber_ - 1);

    QString color;
    QString type;
    if (victory == GameState::NA) {
        gameState_.getLegalMoves(legalMoves_, promotionMoves_);

        whiteTurn_ = !whiteTurn_;

        // TODO Stop clock earlier
        pressClock();
        clearSelectedSquare();
        updatePosition();
        updateClocks();

        if (!twoPlayer_ && whiteVsComputer_ == trueMoveNumber_ % 2) {
            engine_.getMove(move);
        }
        return;
    } else if (victory == GameState::WhiteCheckmate || victory == GameState::BlackCheckmate) {
        color = victory == GameState::WhiteCheckmate ? "White Wins!" : "Black Wins!";
        type = "by Checkmate";
    } else {
        color = "It's a Draw!";

        if (victory == GameState::DrawRepetition)
            type = "by Threefold Repetition";
        else if (victory == GameState::DrawFifty)
            type = "by Fifty-Move Rule";
        else if (victory == GameState::DrawMaterial)
            type = "by Insufficient Material";
        else
            type = "by Stalemate";
    }

    emit gameEnded(color, type);
    emit pauseTimer(whiteTurn_, true);

    gameOver_ = true;
    clearSelectedSquare();
    updatePosition();
}

void Game::pressClock()
{
    if (trueMoveNumber_ == 2)
        emit startTimer(whiteTurn_);
    else if (trueMoveNumber_ > 2) {
        emit startTimer(whiteTurn_);
        emit pauseTimer(!whiteTurn_);
    }
}

void Game::updatePosition()
{
    Position position = gameHistory_[shownMoveNumber_];
    Piece piece;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            piece = position.board[indexAdjustment(row)]
                                  [indexAdjustment(col)];
            emit setPiece(row, col, piece);
        }
    }
}

void Game::updateClocks()
{
    if (!twoPlayer_ && !whiteVsComputer_) {
        emit updateTimerLabels(whiteTimerText_, true);
        emit updateTimerLabels(blackTimerText_, false);
    } else if (flipBoard_) {
        emit updateTimerLabels(whiteTimerText_, !whiteTurn_);
        emit updateTimerLabels(blackTimerText_, whiteTurn_);
    } else {
        emit updateTimerLabels(whiteTimerText_, false);
        emit updateTimerLabels(blackTimerText_, true);
    }
}

int Game::indexAdjustment(int rowOrColIndex) const
{
    if (!twoPlayer_ && !whiteVsComputer_)
        return 7 - rowOrColIndex;
    if (flipBoard_)
        return whiteTurn_ ? rowOrColIndex : 7 - rowOrColIndex;
    else
        return rowOrColIndex;
}

bool Game::vectorContains(int from[2], int to[2], const std::vector<Move> &moveVector) const
{
    Move target = Move(from, to);

    int lowerBound = 0;
    int upperBound = moveVector.size() - 1;
    int index = upperBound / 2;

    while (lowerBound <= upperBound) {
        if (target == moveVector[index])
            return true;

        if (target < moveVector[index])
            upperBound = index - 1;
        else
            lowerBound = index + 1;

        index = (lowerBound + upperBound) / 2;
    }

    return false;
}
