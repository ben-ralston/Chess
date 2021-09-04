#ifndef CHESS_LAYOUT_H
#define CHESS_LAYOUT_H

#include <cmath>

#include <QLayout>
#include <QWidget>
#include <QRect>

class ChessLayout : public QLayout
{
public:
    enum Region { West, North, South, East, Board, Promotion };

    explicit ChessLayout(QWidget *parent, const QMargins &margins = QMargins(), int spacing = -1);
    ~ChessLayout();

    void addLayout(QLayout *layout, Region region);
    void addWidget(QWidget *widget, Region region, int row = -1, int col = -1);
    void addItem(QLayoutItem *item) override;
    int count() const override;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    enum SizeType { MinimumSize, SizeHint };
    struct ItemWrapper
        {
            ItemWrapper(QLayoutItem *i, Region r, int row = -1, int col = -1) {
                item = i;
                region = r;
                this->row = row;
                this->col = col;
            }

            QLayoutItem *item;
            Region region;
            int row;
            int col;
        };

    void add(QLayoutItem *item, Region region, int row = -1, int col = -1);
    QSize calculateSize(SizeType sizeType) const;

    const int topHeight = 50;
    const int sideMinWidth = 100;
    QList<ItemWrapper *> items_;
};

#endif // CHESS_LAYOUT_H
