#include "square.h"

#include <cmath>

#include <QPainter>
#include <QPaintEvent>

using namespace std;

const QImage * const Square::images_ = Square::loadImages();
QColor Square::lightColor_(238, 238, 210);
QColor Square::darkColor_(118, 150, 86);
QColor Square::highlightColor_(192, 204, 60);

Square::Square(QWidget *parent, int row, int col)
    : QWidget(parent),
      row_(row),
      col_(col),
      lightSquare_((row + col) % 2 == 0),
      piece_(None),
      highlighted_(false)
{

}

Square::~Square()
{
    delete [] images_;
}

QImage *Square::loadImages()
{
    QString name_template = "/Users/benralston/Programming/Chess_root/Chess/Images/%1.png";
    QString filename;
    QImage *images = new QImage[12];

    for (int i = 0; i < 12; i++) {
        filename = name_template.arg(i);
        images[i] = QImage(filename);
    }

    return images;
}

void Square::setPiece(int row, int col, Piece newPiece)
{
    if (row_ == row && col_ == col) {
        piece_ = newPiece;
        update();
    }
}

void Square::setHighlight(int row, int col)
{
    if (row_ == row && col_ == col) {
        highlighted_ = !highlighted_;
        update();
    }
}

void Square::mousePressEvent(QMouseEvent *event)
{
    // TODO Handle left and right mouse clicks separately
    emit clicked(row_, col_);
}

void Square::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor color;
    if (highlighted_)
        color = highlightColor_;
    else
        color = lightSquare_ ? lightColor_ : darkColor_;

    painter.fillRect(event->rect(), color);

    if (piece_ == None)
        return;

    int height = event->rect().height();
    float scale = .85;
    int offset = static_cast<int>(round((1 - scale) * height / 2));
    int length = height - 2 * offset;
    QRect target = QRect(offset, offset, length, length);

    painter.drawImage(target, images_[piece_]);
}
