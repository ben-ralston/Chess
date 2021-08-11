#ifndef CHESSLAYOUT_H
#define CHESSLAYOUT_H

#include <QLayout>
#include <QWidget>
#include <QRect>
#include <math.h>

class ChessLayout : public QLayout
{
public:
    enum Region { West, North, South, East, Board };

    explicit ChessLayout(QWidget *parent, const QMargins &margins = QMargins(), int spacing = -1);
    ChessLayout(int spacing = -1);
    ~ChessLayout();

    void addItem(QLayoutItem *item) override;
    void addWidget(QWidget *widget, Region position);
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

    void add(QLayoutItem *item, Region position);

private:
    struct ItemWrapper
    {
        ItemWrapper(QLayoutItem *i, Region p) {
            item = i;
            position = p;
        }

        QLayoutItem *item;
        Region position;
    };

    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;
    QWidget *north, *south, *west, *east, *center;

    QList<ItemWrapper *> list;
};


#endif // CHESSLAYOUT_H
