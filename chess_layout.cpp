#include "chess_layout.h"

#include <QList>

ChessLayout::ChessLayout(QWidget *parent, const QMargins &margins, int spacing) :
    QLayout(parent)
{
    setContentsMargins(margins);
    setSpacing(spacing);
}

ChessLayout::~ChessLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void ChessLayout::addLayout(QLayout *layout, Region region)
{
    add(layout, region);
}

void ChessLayout::addWidget(QWidget *widget, Region region, int row, int col)
{
    add(new QWidgetItem(widget), region, row, col);
}

void ChessLayout::addItem(QLayoutItem *item)
{
    add(item, West);
}

int ChessLayout::count() const
{
    return items_.size();
}

Qt::Orientations ChessLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool ChessLayout::hasHeightForWidth() const
{
    return false;
}

QLayoutItem *ChessLayout::itemAt(int index) const
{
    ItemWrapper *wrapper = items_.value(index);
    return wrapper ? wrapper->item : nullptr;
}

QSize ChessLayout::minimumSize() const
{
    return calculateSize(MinimumSize);
}

void ChessLayout::setGeometry(const QRect &rect)
{
    QMargins margins = contentsMargins();
    int verticalSpacing = 2 * spacing() + margins.top() + margins.bottom();
    int horizontalSpacing = 2 * spacing() + margins.left() + margins.right();

    int centerLength;
    if (rect.width() - 2 * sideMinWidth_ - horizontalSpacing > rect.height() - 2 * topHeight_ - verticalSpacing)
        centerLength = rect.height() - 2 * topHeight_ - verticalSpacing;
    else
        centerLength = rect.width() - 2 * sideMinWidth_ - horizontalSpacing;

    centerLength -= centerLength % 8;
    int squareLength = centerLength / 8;

    float twiceVerticalOffset = static_cast<float>(rect.height() - centerLength - verticalSpacing - 2 * topHeight_);
    int verticalOffset = static_cast<int>(round(twiceVerticalOffset / 2.));

    float twiceSideWidth = static_cast<float>(rect.width() - centerLength - horizontalSpacing);
    int sideWidth = static_cast<int>(round(twiceSideWidth / 2.));

    int boardLeft = margins.left() + sideWidth + spacing();
    int boardTop = margins.top() + topHeight_ + spacing() + verticalOffset;

    float promotionSquareScale = 1.25;
    int promotionSquareLength = static_cast<int>(round(squareLength * promotionSquareScale));
    int promotionSquareLeft = boardLeft + centerLength / 2 - promotionSquareLength;
    int promotionSquareTop = boardTop + centerLength / 2 - promotionSquareLength;

//    float endScreenXScale = 5;
//    float endScreenYScale = 3;
//    int endScreenXLength = static_cast<int>(round(squareLength * endScreenXScale));
//    int endScreenYLength = static_cast<int>(round(squareLength * endScreenYScale));

    float XToYRatio = 1.5;
    int endScreenXLength = 200;
    int endScreenYLength = static_cast<int>(round(endScreenXLength / XToYRatio));
    int endScreenLeft = boardLeft + centerLength / 2 - endScreenXLength / 2;
    int endScreenTop = boardTop + centerLength / 2 - endScreenYLength / 2;

    QLayout::setGeometry(rect);

    for (int i = 0; i < items_.size(); ++i) {
        ItemWrapper *wrapper = items_.at(i);
        QLayoutItem *item = wrapper->item;
        Region region = wrapper->region;

        if (region == West)
            item->setGeometry(QRect(rect.x() + margins.left(), rect.y() + margins.top(),
                                    sideWidth, rect.height() - margins.top() - margins.bottom()));
        else if (region == North)
            item->setGeometry(QRect(rect.x() + margins.left() + sideWidth + spacing(), rect.y() + margins.top(),
                                     centerLength, topHeight_));
        else if (region == South)
            item->setGeometry(QRect(rect.x() + margins.left() + sideWidth + spacing(),
                                    rect.y() + margins.top() + topHeight_ + centerLength + 2 * spacing() + 2 * verticalOffset,
                                    centerLength, topHeight_));
        else if (region == East)
            item->setGeometry(QRect(rect.x() + margins.left() + sideWidth + centerLength + 2 * spacing(),
                                    rect.y() + margins.top(),
                                    sideWidth, rect.height() - margins.top() - margins.bottom()));
        else if (region == Board)
            item->setGeometry(QRect(margins.left() + sideWidth + spacing() + squareLength * wrapper->col,
                                   margins.top() + topHeight_ + spacing() + verticalOffset + squareLength * wrapper->row,
                                   squareLength, squareLength));
        else if (region == Promotion)
            item->setGeometry(QRect(promotionSquareLeft + promotionSquareLength * wrapper->col,
                                    promotionSquareTop + promotionSquareLength * wrapper->row,
                                    promotionSquareLength, promotionSquareLength));
        else if (region == EndScreen)
            item->setGeometry(QRect(endScreenLeft, endScreenTop,
                                    endScreenXLength, endScreenYLength));
    }
}

QSize ChessLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *ChessLayout::takeAt(int index)
{
    if (index >= 0 && index < items_.size()) {
        ItemWrapper *layoutStruct = items_.takeAt(index);
        return layoutStruct->item;
    }
    return nullptr;
}

void ChessLayout::add(QLayoutItem *item, Region region, int row, int col)
{
    items_.append(new ItemWrapper(item, region, row, col));
}

QSize ChessLayout::calculateSize(SizeType sizeType) const
{
    QSize totalSize;

    if (sizeType == MinimumSize) {
        QMargins margins = contentsMargins();
        int squareMinLength = 40;

        totalSize.rheight() += 2 * spacing() + margins.top() + margins.bottom();
        totalSize.rheight() += 2 * topHeight_;
        totalSize.rheight() += 8 * squareMinLength;

        totalSize.rwidth() += 2 * spacing() + margins.left() + margins.right();
        totalSize.rwidth() += 2 * sideMinWidth_;
        totalSize.rwidth() += 8 * squareMinLength;
    } else {
        totalSize.rheight() = 0;
        totalSize.rwidth() = 0;
    }

    return totalSize;

    // TODO Implement item-based size calculations

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
}
