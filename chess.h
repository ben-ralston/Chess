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
    void updateTimerLabels(const QString &text, bool top);

signals:
    void keyPress(int key);

private:
    void keyPressEvent(QKeyEvent *event) override;

    Ui::Chess *ui_;
    Game *game_;
    ChessLayout *layout_;
    Timer *whiteTimer_;
    Timer *blackTimer_;
    QLabel *topTimerLabel_;
    QLabel *bottomTimerLabel_;
    QString topTimerText_;
    QString bottomTimerText_;
};

#endif // CHESS_H
