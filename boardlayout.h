#ifndef BOARDLAYOUT_H
#define BOARDLAYOUT_H

#include <QLayout>
#include <QWidget>
#include <QRect>
#include "square.h"

class BoardLayout : public QLayout
{
public:
    BoardLayout(QWidget *parent);
     ~BoardLayout();
    void setGeometry(const QRect &rect) override;
    void addSquare(Square *square, int row, int col);

    void addItem(QLayoutItem *item) override;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    QLayoutItem *squares[64];
    bool initialized;
};

#endif // BOARDLAYOUT_H
