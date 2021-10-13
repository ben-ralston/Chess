#ifndef PROMOTION_SQUARE_H
#define PROMOTION_SQUARE_H

#include <QObject>

#include "square.h"
#include "chess/piece.h"

class PromotionSquare : public Square
{
    Q_OBJECT

public:
    explicit PromotionSquare(QWidget *parent, int row, int col);

public slots:
    void setColor(bool white);
    void setVisibility(bool visible);

signals:
    void promotionPiece(Piece piece);

private:
    void mousePressEvent(QMouseEvent *event) override;
    static const Piece whitePieces_[4];
    static const Piece blackPieces_[4];
};

#endif // PROMOTION_SQUARE_H
