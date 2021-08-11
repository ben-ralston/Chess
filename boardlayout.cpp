#include "boardlayout.h"

BoardLayout::BoardLayout(QWidget *parent) : QLayout(parent)
{

}

BoardLayout::~BoardLayout() {

}

void BoardLayout::addSquare(Square *square, int row, int col) {
    squares[8 * row + col] = square;
}

void BoardLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);

    int length = floor(rect.width() / 8);

    Square *cur;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cur = squares[8 * row + col];
            cur->setGeometry(QRect(rect.x() + length * col, rect.y() + length * row, length, length));
        }
    }
}
