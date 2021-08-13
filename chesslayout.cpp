#include "chesslayout.h"

ChessLayout::ChessLayout(QWidget *parent, const QMargins &margins, int spacing)
    : QLayout(parent)
{
    setContentsMargins(margins);
    setSpacing(spacing);

    QWidget *w, *e, *n, *s;

    w = new QWidget(parent);
    w->setStyleSheet("background-color:red;");
    w->show();

    west = new QWidgetItem(w);

    e = new QWidget(parent);
    e->setStyleSheet("background-color:blue;");
    e->show();

    east = new QWidgetItem(e);

    n = new QWidget(parent);
    n->setStyleSheet("background-color:green;");
    n->show();

    north = new QWidgetItem(n);

    s = new QWidget(parent);
    s->setStyleSheet("background-color:purple;");
    s->show();

    south = new QWidgetItem(s);
}

ChessLayout::ChessLayout(int spacing)
{
    setSpacing(spacing);
}


ChessLayout::~ChessLayout()
{
    delete north;
    delete south;
    delete west;
    delete east;

    for (int i = 0; i < 64; i++) {
        delete squares[i];
    }
}

void ChessLayout::addItem(QLayoutItem *item)
{
    add(item, West);
}

void ChessLayout::addWidget(QWidget *widget, Region position)
{
    add(new QWidgetItem(widget), position);
}

void ChessLayout::addSquare(Square *square, int row, int col) {
    squares[8 * row + col] = new QWidgetItem(square);
}

Qt::Orientations ChessLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool ChessLayout::hasHeightForWidth() const
{
    return false;
}

int ChessLayout::count() const
{
    return 69;
}

QLayoutItem *ChessLayout::itemAt(int index) const
{
    switch (index) {
    case 0:
        return west;
    case 1:
        return north;
    case 2:
        return east;
    case 3:
        return south;
    }

    if (index < 0 || index > 67)
        return nullptr;

    return squares[index - 4];
}

QSize ChessLayout::minimumSize() const
{
    return calculateSize(MinimumSize);
}

void ChessLayout::setGeometry(const QRect &rect)
{
    int sideMinWidth = 100;
    int topMinHeight = 50;
    int centerMinLength = 200;

    bool heightLimit;
    if (rect.width() - 2 * sideMinWidth > rect.height() - 2 * topMinHeight)
        heightLimit = true;
    else
        heightLimit = false;

    int centerLength;
    int sideWidth;
    int topHeight;

    if (heightLimit) {
        centerLength = rect.height() - 2 * topMinHeight;
        centerLength -= centerLength % 8;
        topHeight = (int) round((rect.height() - centerLength) / 2);
        sideWidth = (int) round((rect.width() - centerLength) / 2);
    } else {
        centerLength = rect.width() - 2 * sideMinWidth;
        centerLength -= centerLength % 8;
        topHeight = (int) round((rect.height() - centerLength) / 2);
        sideWidth = (int) round((rect.width() - centerLength) / 2);
    }

    QLayout::setGeometry(rect);

    west->setGeometry(QRect(rect.x(), rect.y(), sideWidth, rect.height()));
    east->setGeometry(QRect(rect.x() + sideWidth + centerLength, rect.y(), sideWidth, rect.height()));

    north->setGeometry(QRect(rect.x() + sideWidth, rect.y(), centerLength, topHeight));
    south->setGeometry(QRect(rect.x() + sideWidth, rect.y() + topHeight + centerLength,
                             centerLength, topHeight));

    int length = (int) centerLength / 8;

    QLayoutItem *cur;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cur = squares[8 * row + col];
            cur->setGeometry(QRect(sideWidth + length * col, topHeight + length * row, length, length));
        }
    }
}

QSize ChessLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *ChessLayout::takeAt(int index)
{
    return nullptr;
    // TODO implement method
}

void ChessLayout::add(QLayoutItem *item, Region position)
{
    switch (position) {
    case North:
        north = item;
        break;
    case South:
        south = item;
        break;
    case East:
        east = item;
        break;
    case West:
        west = item;
        break;
    }
}

QSize ChessLayout::calculateSize(SizeType sizeType) const
{
    QSize totalSize;

    totalSize.rheight() = 0;
    totalSize.rwidth() = 0;

//    QLayoutItem *item;
//    QSize itemSize;
//    for (int i = 0; i < count(); ++i) {
//        item = itemAt(i);

//        if (sizeType == MinimumSize)
//            itemSize = item->minimumSize();
//        else // (sizeType == SizeHint)
//            itemSize = item->sizeHint();

//        totalSize.rheight() += itemSize.height();

//        totalSize.rwidth() += itemSize.width();
//    }

    return totalSize;
}
