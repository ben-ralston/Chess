#ifndef BOARDCOLORSQUARE_H
#define BOARDCOLORSQUARE_H

#include <QWidget>

class BoardColorSquare : public QWidget
{
    Q_OBJECT

public:
    explicit BoardColorSquare(QWidget *parent, int row, bool left, QColor color);
    const QColor &color() const;
    void setColor(const QColor &newColor);
    QSize sizeHint() const override;

public slots:
    void setSelected(int row);

signals:
    void clicked(int row);
    void selectedColor(QColor color, bool primary);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    int row_;
    bool left_;
    bool selected_;
    QColor color_;
};

#endif // BOARDCOLORSQUARE_H
