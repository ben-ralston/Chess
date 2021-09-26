#ifndef PROMOTIONSQUARE_H
#define PROMOTIONSQUARE_H

#include <QObject>

#include "square.h"
#include "piece.h"

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

#endif // PROMOTIONSQUARE_H