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

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QObject *parent, int whiteTime, int blackTime, int whiteIncrement, int blackIncrement);
    ~Game();
    void updatePosition();
    void updateClocks();
    void setTimeControl(int whiteTime, int blackTime, int whiteIncrement, int blackIncrement);
    void setFlipBoard(bool newFlipBoard);

public slots:
    void completePromotion(Piece piece);
    void expiredTime(bool white);
    void keyPress(int key);
    void mousePress(int row, int col);
    void resetGame();
    void updateShownMove(int move);
    void updateTimerText(const QString &text, bool white);

signals:
    void gameEnded(const QString &color, const QString &victoryType);
    void highlightSquare(int pos[2]);
    void setPiece(int row, int col, Piece piece);
    void setPromotionColor(bool white);
    void setPromotionVisibilty(bool visible);
    void startTimer(bool white);
    void pauseTimer(bool white, bool noIncrement = false);
    void resetTimer(bool white, int startingTime, int increment);
    void updateTimerLabels(const QString &text, bool top);
    void notateMove(const QString &move);
    void clearNotation();
    void notationMoveNumber(int move);

private:
    bool vectorContains(int from[2], int to[2], const std::vector<Move> &moveVector) const;

//    bool lessThan(const int a[4], const int b[4]) const;
//    bool equal(const int a[4], const int b[4]) const;

    void clearSelectedSquare();

    int indexAdjustment(int rowOrColIndex) const;



    void pressClock();
    void rotateSelectedSquare();
    void setSelectedSquare(int row, int col);

    void updateGameInfo();


    const Piece startingPosition_[8][8] = {
        { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook },
        { BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { None, None, None, None, None, None, None, None },
        { WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn },
        { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook },
    };

    GameState gameState_;
    std::vector<Move> legalMoves_;
    std::vector<Move> promotionMoves_;
    std::vector<Position> gameHistory_;
    bool whiteTurn_;
    bool gameOver_;
    int selected_[2];
    int shownMoveNumber_;
    int trueMoveNumber_;

    QString whiteTimerText_;
    QString blackTimerText_;
    bool choosingPromotionPiece_;
    int savedFrom_[2];
    int savedTo_[2];

    int whiteTime_;
    int blackTime_;
    int whiteIncrement_;
    int blackIncrement_;
    bool flipBoard_;
};

#endif // GAME_H
