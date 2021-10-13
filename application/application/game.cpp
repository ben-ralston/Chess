#include "game.h"

#include <array>
#include <algorithm>
#include <iostream>

#include <QKeyEvent>

#include "chess/piece.h"
#include "position/position.h"
#include "position/game_state.h"
#include "position/move.h"

using namespace std;

Game::Game(QObject *parent, int whiteTime, int blackTime, int whiteIncrement, int blackIncrement) :
    QObject(parent),
    selected_{-1, -1},
    flipBoard_(true)
{
    setTimeControl(whiteTime, blackTime, whiteIncrement, blackIncrement);
    resetGame();
}

Game::~Game()
{
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
//    clearSelectedSquare();
}

void Game::updateClocks() {
    if (flipBoard_) {
        emit updateTimerLabels(whiteTimerText_, !whiteTurn_);
        emit updateTimerLabels(blackTimerText_, whiteTurn_);
    } else {
        emit updateTimerLabels(whiteTimerText_, false);
        emit updateTimerLabels(blackTimerText_, true);
    }
}

void Game::completePromotion(Piece piece)
{
    emit setPromotionVisibilty(false);
    choosingPromotionPiece_ = false;

    QString notation = gameState_.makeMovePublic(savedFrom_, savedTo_, piece);
    emit notateMove(notation);

    updateGameInfo();
}

void Game::expiredTime(bool white)
{
    // TODO Implement variable behavior based on winning color
    QString color = white ? "Black Wins!" : "White Wins!";
    QString type = "on Time";
    emit gameEnded(color, type);

    emit pauseTimer(whiteTurn_, true);
    clearSelectedSquare();
    updatePosition();
    gameOver_ = true;
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

void Game::mousePress(int row, int col)
{
    if (gameOver_)
        return;
    if (shownMoveNumber_ != trueMoveNumber_)
        return;

    if (choosingPromotionPiece_) {
        choosingPromotionPiece_ = false;
        emit setPromotionVisibilty(false);
        clearSelectedSquare();
        return;
    }

    int adjRow = indexAdjustment(row);
    int adjCol = indexAdjustment(col);
    bool selectable = gameState_.isSelectable(adjRow, adjCol);

    if (selected_[0] == row && selected_[1] == col) {
        clearSelectedSquare();
        return;
    }
    if (selectable) {
        setSelectedSquare(row, col);
        return;
    }
    if (selected_[0] == -1)
        return;

    int from[2] = { indexAdjustment(selected_[0]), indexAdjustment(selected_[1]) };
    int to[2] = {adjRow, adjCol};

    if (std::find(promotionMoves_.begin(), promotionMoves_.end(), Move(from, to)) != promotionMoves_.end()) {
//    if (vectorContains(from, to, promotionMoves_)) {
        savedFrom_[0] = from[0];
        savedFrom_[1] = from[1];
        savedTo_[0] = to[0];
        savedTo_[1] = to[1];

        choosingPromotionPiece_ = true;

        emit setPromotionColor(whiteTurn_);
        emit setPromotionVisibilty(true);
    } else if (std::find(legalMoves_.begin(), legalMoves_.end(), Move(from, to)) != legalMoves_.end()) {
//    } else if (vectorContains(from, to, legalMoves_)){
        QString notation = gameState_.makeMovePublic(from, to, None);
        emit notateMove(notation);

        updateGameInfo();
    } else
        clearSelectedSquare();

}

void Game::resetGame()
{
    gameOver_ = false;
    whiteTurn_ = true;
    clearSelectedSquare();
    shownMoveNumber_ = 0;
    trueMoveNumber_ = 0;
    choosingPromotionPiece_ = false;

    gameState_.reset();

    // TODO Pass legalMoves_ in as parameter
    legalMoves_ = gameState_.getLegalMoves();
    promotionMoves_ = gameState_.getPromotionMoves(legalMoves_);

    gameHistory_.clear();
    gameHistory_.push_back(gameState_.savePosition());

    emit resetTimer(true, whiteTime_, whiteIncrement_);
    emit resetTimer(false, blackTime_, blackIncrement_);
    emit clearNotation();
    emit notationMoveNumber(-1);
    emit setPromotionVisibilty(false);

    updatePosition();
    updateClocks();
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

//bool Game::lessThan(const int a[4], const int b[4]) const
//{
//    if (a[0] < b[0])
//        return true;
//    if (a[1] < b[1])
//        return true;
//    if (a[2] < b[2])
//        return true;
//    return a[3] < b[3];
//}

//bool Game::equal(const int a[4], const int b[4]) const
//{
//    for (int i = 0; i < 4; i++) {
//        if (a[i] != b[i])
//            return false;
//    }

//    return true;
//}



void Game::clearSelectedSquare()
{
    if (selected_[0] == -1)
        return;

    emit highlightSquare(selected_);
    selected_[0] = -1;
    selected_[1] = -1;
}



int Game::indexAdjustment(int rowOrColIndex) const
{
    if (flipBoard_)
        return whiteTurn_ ? rowOrColIndex : 7 - rowOrColIndex;
    else
        return rowOrColIndex;
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

void Game::rotateSelectedSquare()
{
    if (selected_[0] == -1)
        return;
    setSelectedSquare(7 - selected_[0], 7 - selected_[1]);
}



void Game::setSelectedSquare(int row, int col)
{
    clearSelectedSquare();
    selected_[0] = row;
    selected_[1] = col;
    emit highlightSquare(selected_);
}



void Game::updateGameInfo()
{
    gameHistory_.push_back(gameState_.savePosition());
    GameState::VictoryType victory = gameState_.getOutcome(gameHistory_);

    shownMoveNumber_++;
    trueMoveNumber_++;
    emit notationMoveNumber(shownMoveNumber_ - 1);

    QString color;
    QString type;
    if (victory == GameState::NA) {
        legalMoves_ = gameState_.getLegalMoves();
        promotionMoves_ = gameState_.getPromotionMoves(legalMoves_);

        whiteTurn_ = !whiteTurn_;

        // TODO Stop clock earlier
        pressClock();
        clearSelectedSquare();
        updatePosition();
        updateClocks();
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
