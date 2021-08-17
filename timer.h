#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QString>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr, int time = 300000, int inc = 0, bool isWhite = true);
    void setText();

public slots:
    void start(bool white);
    void pause(bool white);
    void reset(int time = 300000, int inc = 0, bool white = true);
    void updateTime();

signals:
    void changeText(QString text, bool white);
    void expiredTime(bool white);

private:
    QTimer *timer;
    int remainingTime;
    int increment;
    bool white;
    bool adjustment;
    bool started;
    QString createText();
};

#endif // TIMER_H
