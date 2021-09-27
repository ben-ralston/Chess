#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QString>

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject *parent, bool white, int startingTime = 300000, int increment = 0);
    ~Timer();
    void updateText();

public slots:
    void start(bool white);
    void pause(bool white, bool noIncrement);
    void reset(bool white, int startingTime = 300000, int increment = 0);

private slots:
    void updateTime();
    void restartedUpdateTime();

signals:
    void currentTimeText(const QString &text, bool white);
    void expiredTime(bool white);

private:
    const bool white_;
    int remainingTime_;
    int increment_;
    QTimer *mainTimer_;
    QTimer *altTimer_;
};

#endif // TIMER_H
