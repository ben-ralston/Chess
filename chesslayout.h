#ifndef CHESSLAYOUT_H
#define CHESSLAYOUT_H

#include <QLayout>
#include <QWidget>
#include <QRect>
#include <math.h>
#include "square.h"

class ChessLayout : public QLayout
{
public:
    enum Region { West, North, South, East };

    explicit ChessLayout(QWidget *parent, const QMargins &margins = QMargins(), int spacing = -1);
    ChessLayout(int spacing = -1);
    ~ChessLayout();

    void addWidget(QWidget *widget, Region position);
    void addSquare(Square *square, int row, int col);
    void add(QLayoutItem *item, Region position);

    void addItem(QLayoutItem *item) override;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;
    QLayoutItem *north, *south, *west, *east;
    QLayoutItem *squares[64];
};


#endif // CHESSLAYOUT_H
