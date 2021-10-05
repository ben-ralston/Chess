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
    setStyleSheet("background-color: blue;");
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

    if (selected_){
        int x = event->rect().left();
        int y = event->rect().top();
        int width = event->rect().width();
        int height = event->rect().height();

        QRect topRect = QRect(x, y, width, 3);
        QRect bottomRect = QRect(x, y + height - 3, width, 3);

        QRect sideRect;
        if (left_)
            sideRect = QRect(x, y, 3, height);
        else
            sideRect = QRect(x + width - 3, y , 3, height);

        painter.fillRect(topRect, QColorConstants::White);
        painter.fillRect(bottomRect, QColorConstants::White);
        painter.fillRect(sideRect, QColorConstants::White);
    }
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
