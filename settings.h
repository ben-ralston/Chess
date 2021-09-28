#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Settings;
}
QT_END_NAMESPACE

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui_;
};

#endif // SETTINGS_H
