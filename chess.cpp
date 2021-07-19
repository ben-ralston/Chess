#include <iostream>

#include "chess.h"
#include "ui_chess.h"
#include "square.h"

using namespace std;

Chess::Chess(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::Chess)
{
    ui->setupUi(this);

    game = Game();

    selected[0] = -1;
    selected[1] = -1;

    QWidget *board = this->findChild<QWidget *>("board");
    Square *square;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            square = new Square(board, row, col);
            connect(square, &Square::clicked, this, &Chess::mousePress);
            connect(this, &Chess::setPiece, square, &Square::setPiece);
            connect(this, &Chess::highlight, square, &Square::setHighlight);
        }
    }

    updatePosition();
}

Chess::~Chess()
{
    delete ui;
}

void Chess::mousePress(int r, int c) {
    bool selectable = game.isSelectable(r, c);

    if (selected[0] == -1 && selectable) {
        setSelected(r, c);
    } else if (selected[0] == r && selected[1] == c) {
        clearSelected();
    } else if (selectable) {
        setSelected(r, c);
    } else if (selected[0] != -1) {
        int from[2] = {selected[0], selected[1]};
        int to[2] = {r, c};
        game.tryMove(from, to);
        clearSelected();
        updatePosition();
    }
}

void Chess::updatePosition() {
    Piece * ptr = game.getPosition();
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            emit setPiece(r, c, ptr[8 * r + c]);
        }
    }
 }

void Chess::clearSelected() {
    if (selected[0] == -1) return;
    emit highlight(selected[0], selected[1]);
    selected[0] = -1;
    selected[1] = -1;
}

void Chess::setSelected(int r, int c) {
    clearSelected();
    selected[0] = r;
    selected[1] = c;
    emit highlight(r, c);
}
