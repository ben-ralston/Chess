#include "timer.h"

Timer::Timer(QObject *parent, int time, int inc, bool isWhite) : QObject(parent)
{
    remainingTime = time;
    increment = inc;
    white = isWhite;
    adjustment = false;
    started = false;

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Timer::updateTime);
}

void Timer::start(bool white) {
    if (this->white != white)
        return;

    if (remainingTime % 100 == 0) {
        timer->start(100);
    } else {
        timer->start(remainingTime % 100);
        adjustment = true;
    }

//    if (started)
//        remainingTime += increment;
//    else
//        started = true;
}

void Timer::pause(bool white) {
    if (this->white != white)
        return;

    int timeLeft = timer->remainingTime();
    timer->stop();

    remainingTime -= 100 - timeLeft;

    remainingTime += increment;
    setText();
}

void Timer::reset(int time, int inc, bool white) {
    if (this->white == white) {
        timer->stop();
        remainingTime = time;
        increment = inc;
        adjustment = false;
        started = false;
        setText();
    }
}

void Timer::updateTime() {
    if (adjustment) {
        remainingTime -= remainingTime % 100;
        adjustment = false;
        timer->start(100);
    }
    else
        remainingTime -= 100;

    setText();

    if (remainingTime <= 0)
        emit expiredTime(white);
}

void Timer::setText() {
    emit changeText(createText(), white);
}

QString Timer::createText() {
    int min = floor(remainingTime / 60000);
    int sec = floor((remainingTime % 60000) / 1000);
    int tenths = ceil((remainingTime % 1000) / 100);

    QString out;
    if (min > 0)
        out = QString("%1:%2").arg(min).arg(sec, 2, 10, QLatin1Char('0'));
    else
        out = QString("0:%1.%2").arg(sec, 2, 10, QLatin1Char('0')).arg(tenths);

    return out;
}
