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

private:
    Square *squares[64];
};

#endif // BOARDLAYOUT_H
