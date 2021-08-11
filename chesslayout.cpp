#include "chesslayout.h"

ChessLayout::ChessLayout(QWidget *parent, const QMargins &margins, int spacing)
    : QLayout(parent)
{
    setContentsMargins(margins);
    setSpacing(spacing);

    west = new QWidget(parent);
    west->setStyleSheet("background-color:red;");
    west->show();

    east = new QWidget(parent);
    east->setStyleSheet("background-color:blue;");
    east->show();

    north = new QWidget(parent);
    north->setStyleSheet("background-color:green;");
    north->show();

    south = new QWidget(parent);
    south->setStyleSheet("background-color:purple;");
    south->show();

    center = new QWidget(parent);
    center->setStyleSheet("background-color:yellow;");
    center->show();

}

ChessLayout::ChessLayout(int spacing)
{
    setSpacing(spacing);
}


ChessLayout::~ChessLayout()
{
    QLayoutItem *l;
    while ((l = takeAt(0)))
        delete l;
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

    center->setGeometry(QRect(rect.x() + sideWidth, rect.y() + topHeight, centerLength, centerLength));
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
    list.append(new ItemWrapper(item, position));
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
