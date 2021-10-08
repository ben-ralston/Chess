#include "timer.h"

Timer::Timer(QObject *parent, bool white) :
    QObject(parent),
    white_(white)
{
    mainTimer_ = new QTimer();
    mainTimer_->setTimerType(Qt::PreciseTimer);
    mainTimer_->setInterval(100);
    connect(mainTimer_, &QTimer::timeout, this, &Timer::updateTime);

    altTimer_ = new QTimer();
    altTimer_->setTimerType(Qt::PreciseTimer);
    altTimer_->setSingleShot(true);
    connect(altTimer_, &QTimer::timeout, this, &Timer::restartedUpdateTime);
}

Timer::~Timer()
{
    mainTimer_->stop();
    altTimer_->stop();
    delete mainTimer_;
    delete altTimer_;
}

void Timer::updateText()
{
    int min = remainingTime_ / 60000;
    int sec = (remainingTime_ % 60000) / 1000;
    int tenths = (remainingTime_ % 1000) / 100 + (remainingTime_ % 100 != 0);

    if (tenths == 10) {
        tenths = 0;
        sec += 1;
        if (sec == 60) {
            sec = 0;
            min += 1;
        }
    }

    QString timeString;
    if (min > 0)
        timeString = QString("%1:%2").arg(min).arg(sec, 2, 10, QLatin1Char('0'));
    else
        timeString = QString("0:%1.%2").arg(sec, 2, 10, QLatin1Char('0')).arg(tenths);

    emit currentTimeText(timeString, white_);
}

void Timer::start(bool white)
{
    if (white_ != white)
        return;

    if (remainingTime_ % 100 == 0)
        mainTimer_->start();
    else
        altTimer_->start(remainingTime_ % 100);
}

void Timer::pause(bool white, bool noIncrement)
{
    if (white_ != white)
        return;

    QTimer *timer;
    if (altTimer_->isActive())
        timer = altTimer_;
    else
        timer = mainTimer_;

    if (noIncrement) {
        timer->stop();
        return;
    }

    int timeLeft = timer->remainingTime();
    timer->stop();
    int timeElapsed = timer->interval() - timeLeft;

    remainingTime_ += increment_ - timeElapsed;
    updateText();
}

void Timer::reset(bool white, int startingTime, int increment)
{
    if (white_ != white)
        return;

    mainTimer_->stop();
    altTimer_->stop();
    remainingTime_ = startingTime;
    increment_ = increment;
    updateText();
}

void Timer::updateTime()
{
    // TODO Prevent window resize events from delaying timer
    remainingTime_ -= 100;
    updateText();

    if (remainingTime_ <= 0)
        emit expiredTime(white_);
}

void Timer::restartedUpdateTime()
{
    remainingTime_ -= remainingTime_ % 100;
    updateText();

    if (remainingTime_ <= 0)
        emit expiredTime(white_);
    else
        mainTimer_->start();
}
