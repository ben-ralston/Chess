#include "notation_model.h"

#include <iostream>
#include <QColor>

NotationModel::NotationModel(QObject *parent) : QAbstractTableModel(parent)
{
    moveNumber_ = -1;
}

int NotationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (moves_.length() == 0)
        return 1;
    return (moves_.length() + 1) / 2;
}

int NotationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant NotationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();
    int moveIndex = 2 * row + col - 1;

    if (role == Qt::DisplayRole) {
        if (col == 0)
            return QString("%1:").arg(row + 1);

//        int moveIndex = 2 * row + col - 1;
        if (moveIndex > moves_.length() - 1)
            return QVariant();

        return moves_[moveIndex];
    } if (role == Qt::BackgroundRole) {
        if (moveIndex == moveNumber_ && col != 0) {
//            return QVariant();
            return QColor(82, 89, 97);
        } else {
            return QColor(41, 44, 48);
        }
    }


    return QVariant();
}

void NotationModel::addMove(const QString &move)
{
    emit layoutAboutToBeChanged();
    moves_.append(move);
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    emit layoutChanged();
    emit scrollTable();
}

void NotationModel::clearMoves()
{
    emit layoutAboutToBeChanged();
    moves_.clear();
    emit layoutChanged();
}

void NotationModel::setMoveNumber(int move)
{
    moveNumber_ = move;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void NotationModel::clickedCell(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();
    int moveIndex = 2 * row + col - 1;
    if (col != 0 && moveIndex < moves_.length()) {
        moveNumber_ = moveIndex;
        emit dataChanged(this->index(0, 0), this->index(rowCount() - 1, columnCount() - 1));
        emit updateShownMove(moveIndex + 1);
    }
}
