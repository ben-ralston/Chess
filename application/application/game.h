#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QKeyEvent>
#include <QString>

#include "chess/piece.h"
#include "rules/position.h"
#include "rules/game_state.h"
#include "rules/move.h"
#include "timer.h"

class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject *parent, int whiteTime, int blackTime, int whiteIncrement, int blackIncrement);
    ~Game();
    void updateGame();
    void setTimeControl(int whiteTime, int blackTime, int whiteIncrement, int blackIncrement);
    void setFlipBoard(bool newFlipBoard);

public slots:
    void mousePress(int row, int col);
    void keyPress(int key);
    void completePromotion(Piece piece);
    void expiredTime(bool white);
    void updateShownMove(int move);
    void updateTimerText(const QString &text, bool white);
    void reset();

signals:
    void setPiece(int row, int col, Piece piece);
    void highlightSquare(int pos[2]);
    void setPromotionColor(bool white);
    void setPromotionVisibilty(bool visible);
    void gameEnded(const QString &color, const QString &victoryType);
    void startTimer(bool white);
    void pauseTimer(bool white, bool noIncrement = false);
    void resetTimer(bool white, int startingTime, int increment);
    void updateTimerLabels(const QString &text, bool top);
    void notateMove(const QString &move);
    void notationMoveNumber(int move);
    void clearNotation();

private:
    void setSelectedSquare(int row, int col);
    void clearSelectedSquare();
    void rotateSelectedSquare();
    void processMove();
    void pressClock();
    void updatePosition();
    void updateClocks();
    int indexAdjustment(int rowOrColIndex) const;
    bool vectorContains(int from[2], int to[2], const std::vector<Move> &moveVector) const;

    GameState gameState_;
    std::vector<Move> legalMoves_;
    std::vector<Move> promotionMoves_;
    std::vector<Position> gameHistory_;
    bool whiteTurn_;
    bool gameOver_;
    int selectedSquare_[2];
    int shownMoveNumber_;
    int trueMoveNumber_;
    bool choosingPromotionPiece_;
    int promotionFrom_[2];
    int promotionTo_[2];

    Timer *whiteTimer_;
    Timer *blackTimer_;
    QString whiteTimerText_;
    QString blackTimerText_;
    int whiteTime_;
    int blackTime_;
    int whiteIncrement_;
    int blackIncrement_;
    bool flipBoard_;
};

#endif // GAME_H
