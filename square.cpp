#include "square.h"

Square::Square(QWidget *parent, bool isLight, int rowNum, int colNum) : QWidget(parent)
{
    light = isLight;
    row = rowNum;
    col = colNum;

    QString rgb;
    if (light) {
        rgb = "background-color: rgb(238, 238, 210)";
    } else {
        rgb = "background-color: rgb(118, 150, 86)";
    }

    this->setStyleSheet(rgb);
}


Square::~Square()
{

}
