#include <iostream>

#include "chess.h"
#include "ui_chess.h"

using namespace std;

Chess::Chess(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Chess)
{
    ui->setupUi(this);

    QWidget *board = this->findChild<QWidget *>("board");
    QWidget *square;
    QString rgb;

    QWidget *squares [8][8];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            square = new QWidget(board);
//            square->setObjectName(QString::fromUtf8("board"));
            square->setGeometry(QRect(64 * row, 64 * col, 64, 64));
            QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(board->sizePolicy().hasHeightForWidth());
            square->setSizePolicy(sizePolicy);
            square->setMinimumSize(QSize(64, 64));
//            board->setCursor(QCursor(Qt::OpenHandCursor));
            if ((row + col) % 2 == 0) {
                rgb = "background-color: rgb(238, 238, 210)";
            } else {
                rgb = "background-color: rgb(118, 150, 86)";
            }
            square->setStyleSheet(rgb);
            squares[row][col] = square;
        }
    }

}

Chess::~Chess()
{
    delete ui;
}

