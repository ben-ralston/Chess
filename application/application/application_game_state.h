#ifndef APPLICATION_GAME_STATE_H
#define APPLICATION_GAME_STATE_H

#include <QString>

#include <rules/game_state.h>
#include <chess/piece.h>

class ApplicationGameState : public GameState
{
public:
    ApplicationGameState();
    bool isSelectable(int row, int col) const;
    QString move(int from[2], int to[2], Piece promotionPiece);

private:
    QString algebraicNotation(int from[2], int to[2], Piece promotionPiece) const;

    QString notationAmbiguity(int from[2], int to[2]) const;
    char colToFile(int col) const;
    char rowToRank(int row) const;
};

#endif // APPLICATION_GAME_STATE_H
