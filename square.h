#ifndef SQUARE_H
#define SQUARE_H

#include <QWidget>
#include <QPainter>

#include "piece.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class Square; }
//QT_END_NAMESPACE

class Square : public QWidget
{
    Q_OBJECT

public:
    explicit Square(QWidget *parent, int rowNum, int colNum, Piece startPiece = None);
    ~Square();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void setPiece(int r, int c, Piece newPiece);
    void setHighlight(int r, int c);

signals:
    void clicked(int row, int col);

private:
    void loadImages();
    int row;
    int col;
    bool highlight;
    QColor base_color;
    Piece piece;
    QImage images[12];
};

#endif // SQUARE_H
