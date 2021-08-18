#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>

#include "game.h"
#include "chess_layout.h"
#include "piece.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Chess; }
QT_END_NAMESPACE

class Chess : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chess(QWidget *parent = nullptr);
    ~Chess();

private slots:
    void newGame();
    void mousePress(int row, int col);
    void updateTimerText(const QString &text, bool white);
    void expiredTime(bool white);

signals:
    void setPiece(int row, int col, Piece piece);
    void highlightSquare(int row, int col);
    void startTimer(bool white);
    void pauseTimer(bool white);
    void resetTimer(int startingTime, int increment, bool white);

private:
    void keyPressEvent(QKeyEvent *event) override;
    void updatePosition();
    void setSelectedSquare(int row, int col);
    void clearSelectedSquare();
    void pressClock();
    void updateTimerLabels();
    int squareIndexAdjustment(int rowOrColIndex) const;

    Ui::Chess *ui_;
    Game game_;
    int selected_[2];
    int shownMoveNumber_;
    int trueMoveNumber_;
    bool whiteTurn_;
    ChessLayout *layout_;
    Timer *whiteTimer_;
    Timer *blackTimer_;
    QLabel *topTimerLabel_;
    QLabel *bottomTimerLabel_;
    QString whiteTimerText_;
    QString blackTimerText_;
};

#endif // CHESS_H
