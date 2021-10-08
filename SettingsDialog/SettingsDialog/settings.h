#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>

struct Settings
{
    static constexpr QColor presetColors[6] = {
        QColor(238, 238, 210), QColor(118, 150, 86),
        QColor(190,161,127), QColor(120, 86, 61),
        QColor(232, 235, 239), QColor(125, 135, 150)
    };

    bool twoPlayer;
    bool flipBoard;
    bool startWhiteVsComputer;
    int whiteTime;
    int blackTime;
    int whiteIncrement;
    int blackIncrement;
    int selectedColorRow;
    QColor primaryCustomColor;
    QColor secondaryCustomColor;
    bool resetGame;
};

#endif // SETTINGS_H
