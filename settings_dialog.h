#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
//#include <QColorConstants>

#include "board_color_square.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SettingsDialog;
}
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, const Settings &settings);
    ~SettingsDialog();
    const Settings &saveSettings();
    void revertSettings();

private slots:
    void humanVHumanClicked();
    void humanVComputerClicked();
    void startWhiteClicked();
    void startBlackClicked();
    void colorSquareClicked(int row);
    void choosePrimaryColor();
    void chooseSecondaryColor();

signals:
    void setSelectedRow(int row);

private:
    void mousePressEvent(QMouseEvent *event) override;

    Ui::SettingsDialog *ui_;
    Settings settings_;
    BoardColorSquare *primaryCustomColorSquare_;
    BoardColorSquare *secondaryCustomColorSquare_;
    int selectedColorRow_;
};

#endif // SETTINGS_DIALOG_H
