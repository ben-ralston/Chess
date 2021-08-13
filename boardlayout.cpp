#include "boardlayout.h"

BoardLayout::BoardLayout(QWidget *parent) : QLayout(parent)
{
    initialized = false;
}

BoardLayout::~BoardLayout() {

}

void BoardLayout::addSquare(Square *square, int row, int col) {
    squares[8 * row + col] = new QWidgetItem(square);
//    if (row == 7 && col == 7)
//        initialized = true;
}

void BoardLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);

    if (!initialized)
        return;

    return;

    int length = floor(rect.width() / 8);

    QLayoutItem *cur;
    QWidget *sq;



    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cur = squares[8 * row + col];
            sq = cur->widget();
            sq->setGeometry(QRect(rect.x() + length * col, rect.y() + length * row, length, length));
        }
    }
}

void BoardLayout::addItem(QLayoutItem *item) {

}

Qt::Orientations BoardLayout::expandingDirections() const {
    return Qt::Horizontal | Qt::Vertical;
}

bool BoardLayout::hasHeightForWidth() const {
    return false;
}

int BoardLayout::count() const {
    return 0;
}

QLayoutItem *BoardLayout::itemAt(int index) const {
    return squares[index];
}

QSize BoardLayout::minimumSize() const {
    QSize min;
    min.rheight() = 0;
    min.rwidth() = 0;
    return min;
}

QSize BoardLayout::sizeHint() const {
    QSize min;
    min.rheight() = 0;
    min.rwidth() = 0;
    return min;
}

QLayoutItem *BoardLayout::takeAt(int index) {
    return squares[index];
}
