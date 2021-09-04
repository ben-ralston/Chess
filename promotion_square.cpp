#include <QGraphicsOpacityEffect>

#include "promotion_square.h"

const Piece PromotionSquare::whitePieces_[4] = { WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight };
const Piece PromotionSquare::blackPieces_[4] = { BlackQueen, BlackRook, BlackBishop, BlackKnight };

PromotionSquare::PromotionSquare(QWidget *parent, int row, int col) : Square(parent, row, col)
{
    lightSquare_ = true;

    QGraphicsOpacityEffect *op = new QGraphicsOpacityEffect(this);
    op->setOpacity(.75);
    setGraphicsEffect(op);
    setAutoFillBackground(true);

    hide();
}

void PromotionSquare::setColor(bool white)
{
    int i = 2 * row_ + col_;
    if (white)
        setPiece(row_, col_, whitePieces_[i]);
    else
        setPiece(row_, col_, blackPieces_[i]);

}

void PromotionSquare::setVisibility(bool visible)
{
    if (visible)
        show();
    else
        hide();
}

void PromotionSquare::mousePressEvent(QMouseEvent *event)
{
    // TODO Handle left and right mouse clicks separately
    emit promotionPiece(piece_);
}
