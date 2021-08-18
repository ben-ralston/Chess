#ifndef SQUARE_H
#define SQUARE_H

#include <QWidget>

#include "piece.h"

class Square : public QWidget
{
    Q_OBJECT

public:
    explicit Square(QWidget *parent, int row, int col);
    ~Square();

public slots:
    void setPiece(int row, int col, Piece piece);
    void setHighlight(int row, int col);

signals:
    void clicked(int row, int col);

private:
    static QImage *loadImages();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    static const QImage * const images_;
    static QColor lightColor_;
    static QColor darkColor_;
    static QColor highlightColor_;
    const int row_;
    const int col_;
    const bool lightSquare_;
    Piece piece_;
    bool highlighted_;
};

#endif // SQUARE_H
