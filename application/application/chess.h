#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>

#include "game.h"
#include "chess_layout.h"
#include "chess/piece.h"
#include "timer.h"
#include "notation_model.h"
#include "settings/settings_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Chess;
}
QT_END_NAMESPACE

class Chess : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chess(QWidget *parent = nullptr);
    ~Chess();

private slots:
    void updateTimerLabels(const QString &text, bool top);
    void gameOver(const QString &color, const QString &victoryType);
    void openSettings();
    void updateSettings(int result);

signals:
    void keyPress(int key);

private:
    void keyPressEvent(QKeyEvent *event) override;
    Settings initialSettings();

    Ui::Chess *ui_;
    Game *game_;
    ChessLayout *layout_;
    Timer *whiteTimer_;
    Timer *blackTimer_;
    NotationModel *model_;
    SettingsDialog *settingsDialog_;
};

#endif // CHESS_H
