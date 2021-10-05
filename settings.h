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
                      const QColor presetColors[6], QColor primaryCustomColor, QColor secondaryCustomColor,
                      int selectedColorRow);
    ~Settings();

    bool applied() const;

private slots:
    void humanVHumanClicked();
    void humanVComputerClicked();
    void startWhiteClicked();
    void startBlackClicked();
    void colorSquareClicked(int row);
    void setColor(QColor color, bool primary);
    void choosePrimaryColor();
    void chooseSecondaryColor();
    void apply();

signals:
    void setSelectedColor(int row);
    void closed(bool applied, bool humanVHuman = false, bool flipBoard = false, bool startWhite = false,
                int whiteTime = 0, int blackTime = 0, int whiteIncrement = 0, int blackIncrement = 0,
                QColor primaryBoard = QColor(), QColor secondaryBoard = QColor(), QColor primaryCustom = QColor(),
                QColor secondaryCustom = QColor(), int selectedRow = 0);

private:
    const QColor startingColors_[8] = { QColorConstants::Yellow, QColorConstants::DarkGreen,
                                        QColorConstants::LightGray, QColorConstants::DarkBlue,
                                        QColorConstants::LightGray, QColorConstants::Red,
                                        QColorConstants::LightGray, QColorConstants::Red,
                                      };
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    Ui::Settings *ui_;
    QColor primaryColor_;
    QColor secondaryColor_;
    BoardColorSquare *customPrimaryColor_;
    BoardColorSquare *customSecondaryColor_;
    int selectedColorRow_;
    bool applied_;
};

#endif // SETTINGS_H
