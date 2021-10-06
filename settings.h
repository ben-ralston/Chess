#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
//#include <QColorConstants>

#include "board_color_square.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Settings;
}
QT_END_NAMESPACE

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent, bool twoPlayer, bool flipBoard, bool startWhiteVsComputer,
                      int whiteTime, int blackTime, int whiteIncrement, int blackIncrement,
                      const QColor (&presetColors)[6], const QColor &primaryCustomColor,
                      const QColor &secondaryCustomColor, int selectedColorRow);
    ~Settings();

private slots:
    void apply();
    void humanVHumanClicked();
    void humanVComputerClicked();
    void startWhiteClicked();
    void startBlackClicked();
    void colorSquareClicked(int row);
    void setColor(const QColor &color, bool primary);
    void choosePrimaryColor();
    void chooseSecondaryColor();

signals:
    void setSelectedColor(int row);
    void closed(bool applied, bool humanVHuman = false, bool flipBoard = false, bool startWhite = false,
                int whiteTime = 0, int blackTime = 0, int whiteIncrement = 0, int blackIncrement = 0,
                const QColor &primaryBoard = QColor(), const QColor &secondaryBoard = QColor(),
                const QColor &primaryCustom = QColor(), const QColor &secondaryCustom = QColor(), int selectedRow = 0);

private:
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    Ui::Settings *ui_;
    BoardColorSquare *primaryCustomColorSquare_;
    BoardColorSquare *secondaryCustomColorSquare_;
    QColor primarySelectedColor_;
    QColor secondarySelectedColor_;
    int selectedColorRow_;
    bool applied_;
};

#endif // SETTINGS_H
