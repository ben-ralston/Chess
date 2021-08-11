#include "square.h"
#include <iostream>
#include <math.h>

using namespace std;

Square::Square(QWidget *parent, int rowNum, int colNum, Piece startPiece) : QWidget(parent)
{
    row = rowNum;
    col = colNum;
    if ((row + col) % 2 == 0) {
        base_color = QColor(238, 238, 210);
    } else {
        base_color = QColor(118, 150, 86);
    }
    piece = startPiece;
    highlight = false;

    int length = floor(parent->width() / 8);

    setGeometry(QRect(length * col, length * row, length, length));
//    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//    sizePolicy.setHorizontalStretch(0);
//    sizePolicy.setVerticalStretch(0);
//    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
//    setSizePolicy(sizePolicy);
//    setMinimumSize(QSize(64, 64));

    loadImages();

}


Square::~Square()
{

}

void Square::mousePressEvent(QMouseEvent *event)
{
    emit clicked(row, col);
}

void Square::paintEvent(QPaintEvent *event)
{
    QColor color;
    QPainter painter(this);
    if (highlight) {
        color = QColor(192, 204, 60);
    } else color = base_color;
    painter.fillRect(this->rect(), color);

    if (piece == -1) {
        return;
    }

    float scale = .85;
    int h = this->height();
    int offset = (int) round((1 - scale) * h / 2);
    int dim = h - 2 * offset;
//    cout << dim << ' ' << this->width() << '\n';
    QRect target = QRect(offset, offset, dim, dim);
    int ind = (int) piece;
//    painter.drawImage(target, images[ind], QRect(0, 0, 504, 504));
    painter.drawImage(target, images[ind]);
//    cout << "Drew " << row << ", " << col << '\n';
//    cout << x() << "  " << y() << "  " << width() << "  " << height() << '\n';
}

void Square::loadImages() {
    QString name_template = "/Users/benralston/Programming/Chess_root/Chess/Images/%1.png";
    QString filename;
    for (int i = 0; i < 12; i++) {
        filename = name_template.arg(i);
        images[i] = QImage(filename);
    }
}

void Square::setPiece(int r, int c, Piece newPiece) {
    if (r == row && c == col)
        piece = newPiece;
    this->update();
}

void Square::setHighlight(int r, int c) {
    if (r == row && c == col)
        highlight = !highlight;
    this->update();
}
