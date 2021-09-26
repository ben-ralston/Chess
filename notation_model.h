#ifndef NOTATIONITEM_H
#define NOTATIONITEM_H

#include <QObject>
#include <QAbstractTableModel>
#include <QString>


class NotationModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NotationModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

public slots:
    void addMove(const QString &move);
    void clearMoves();
    void setMoveNumber(int move);
    void clickedCell(const QModelIndex &index);

signals:
    void updateShownMove(int move);
    void scrollTable();

private:
    QList<QString> moves_;
    int moveNumber_;
};

#endif // NOTATIONITEM_H
