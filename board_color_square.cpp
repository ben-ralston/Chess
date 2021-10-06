#include "board_color_square.h"

#include <QSizePolicy>
#include <QPainter>
#include <QPaintEvent>
//#include <QColorConstants>

BoardColorSquare::BoardColorSquare(QWidget *parent, int row, bool left, QColor color) :
    QWidget(parent),
    row_(row),
    left_(left),
    selected_(false),
    color_(color)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMinimumSize(QSize(40, 40));
}

const QColor &BoardColorSquare::color() const
{
    return color_;
}

void BoardColorSquare::setColor(const QColor &newColor)
{
    color_ = newColor;
    if (selected_)
        emit selectedColor(color_, left_);
    update();
}

QSize BoardColorSquare::sizeHint() const
{
    return QSize(40, 40);
}

void BoardColorSquare::setSelected(int row)
{
    if (row_ == row) {
        selected_ = true;
        emit selectedColor(color_, left_);
    }
    else
        selected_ = false;

    update();
}

void BoardColorSquare::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked(row_);
    QWidget::mouseReleaseEvent(event);
}

void BoardColorSquare::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), color_);

    int x = event->rect().left();
    int y = event->rect().top();
    int width = event->rect().width();
    int height = event->rect().height();

    QRect topRect;
    QRect bottomRect;
    QRect sideRect;
    if (selected_){
        int marginWidth = 4;
        topRect = QRect(x, y, width, marginWidth);
        bottomRect = QRect(x, y + height - marginWidth, width, marginWidth);

        if (left_)
            sideRect = QRect(x, y, marginWidth, height);
        else
            sideRect = QRect(x + width - marginWidth, y , marginWidth, height);

        painter.fillRect(topRect, QColor(60, 60, 60));
        painter.fillRect(bottomRect, QColor(60, 60, 60));
        painter.fillRect(sideRect, QColor(60, 60, 60));
    } else {
        topRect = QRect(x, y, width, 1);
        bottomRect = QRect(x, y + height - 1, width, 1);

        if (left_)
            sideRect = QRect(x, y, 1, height);
        else
            sideRect = QRect(x + width - 1, y , 1, height);

        painter.fillRect(topRect, QColorConstants::Black);
        painter.fillRect(bottomRect, QColorConstants::Black);
        painter.fillRect(sideRect, QColorConstants::Black);
    }
}
