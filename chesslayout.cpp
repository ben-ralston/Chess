#include "chesslayout.h"

ChessLayout::ChessLayout(QWidget *parent, const QMargins &margins, int spacing)
    : QLayout(parent)
{
    setContentsMargins(margins);
    setSpacing(spacing);

    QWidget *w, *e, *n, *s, *b;

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

//    b = new QWidget(parent);
//    b->setStyleSheet("background-color:yellow;");
//    b->show();

//    board = new QWidgetItem(b);

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
    delete board;
}

void ChessLayout::addItem(QLayoutItem *item)
{
    add(item, West);
}

void ChessLayout::addWidget(QWidget *widget, Region position)
{
    add(new QWidgetItem(widget), position);
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
    return list.size();
}

QLayoutItem *ChessLayout::itemAt(int index) const
{
    ItemWrapper *wrapper = list.value(index);
    return wrapper ? wrapper->item : nullptr;
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
        topHeight = topMinHeight;
        sideWidth = (int) round((rect.width() - centerLength) / 2);
    } else {
        centerLength = rect.width() - 2 * sideMinWidth;
        sideWidth = sideMinWidth;
        topHeight = (int) round((rect.height() - centerLength) / 2);
    }

    QLayout::setGeometry(rect);

    west->setGeometry(QRect(rect.x(), rect.y(), sideWidth, rect.height()));
    east->setGeometry(QRect(rect.x() + sideWidth + centerLength, rect.y(), sideWidth, rect.height()));

    north->setGeometry(QRect(rect.x() + sideWidth, rect.y(), centerLength, topHeight));
    south->setGeometry(QRect(rect.x() + sideWidth, rect.y() + topHeight + centerLength,
                             centerLength, topHeight));

//    board->setGeometry(QRect(rect.x() + sideWidth, rect.y() + topHeight, centerLength, centerLength));
}

QSize ChessLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *ChessLayout::takeAt(int index)
{
    if (index >= 0 && index < list.size()) {
        ItemWrapper *layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    }
    return nullptr;
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
    case Board:
        board = item;
        break;
    }
}

QSize ChessLayout::calculateSize(SizeType sizeType) const
{
    QSize totalSize;

    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper *wrapper = list.at(i);
        Region position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize)
            itemSize = wrapper->item->minimumSize();
        else // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();

        if (position == North || position == South || position == Board)
            totalSize.rheight() += itemSize.height();

        if (position == West || position == East || position == Board)
            totalSize.rwidth() += itemSize.width();
    }
    return totalSize;
}
