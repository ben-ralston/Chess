#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>

#include "game.h"
#include "chess_layout.h"
#include "piece.h"
#include "timer.h"
#include "notation_model.h"

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
    void settingsClosed(bool applied, bool twoPlayer, bool flipBoard, bool startWhite,
                        int whiteTime, int blackTime, int whiteIncrement, int blackIncrement,
                        const QColor &primaryColor, const QColor &secondaryColor,
                        const QColor &primaryCustomColor, const QColor &secondaryCustomColor,
                        int selectedRow);

signals:
    void keyPress(int key);

private:
    const QColor settingsPresetColors_[6] = { QColor(238, 238, 210), QColor(118, 150, 86),
                                              QColor(190,161,127), QColor(120, 86, 61),
                                              QColor(232, 235, 239), QColor(125, 135, 150)
                                            };

    void keyPressEvent(QKeyEvent *event) override;

    Ui::Chess *ui_;
    Game *game_;
    ChessLayout *layout_;
    Timer *whiteTimer_;
    Timer *blackTimer_;
    QString topTimerText_;
    QString bottomTimerText_;
    NotationModel *model_;
    bool twoPlayer_;
    bool flipBoard_;
    bool startWhiteVsComputer_;
    int whiteTime_;
    int blackTime_;
    int whiteIncrement_;
    int blackIncrement_;
    QColor primaryCustomColor_;
    QColor secondaryCustomColor_;
    int selectedColorRow_;
};

#endif // CHESS_H
