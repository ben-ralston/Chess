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
    void setHighlight(int pos[2]);

signals:
    void clicked(int row, int col);

protected:
    bool lightSquare_;
    const int row_;
    const int col_;
    Piece piece_;

private:
    static QImage *loadImages();
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    static const QImage * const images_;
    static QColor lightColor_;
    static QColor darkColor_;
    static QColor highlightColor_;
    bool highlighted_;
};

#endif // SQUARE_H
