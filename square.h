#ifndef SQUARE_H
#define SQUARE_H

#include <QWidget>

class Square : public QWidget
{
    Q_OBJECT

public:
    explicit Square(QWidget *parent, bool isLight, int rowNum, int colNum);
    ~Square();
    void mousePressEvent(QMouseEvent *event);

public slots:


signals:
    void clicked(int row, int col);

private:
    bool light;
    int row;
    int col;

};

#endif // SQUARE_H
