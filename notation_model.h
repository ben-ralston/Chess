#ifndef NOTATION_MODEL_H
#define NOTATION_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QString>

class NotationModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NotationModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

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

#endif // NOTATION_MODEL_H
