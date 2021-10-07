QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board_color_square.cpp \
    chess_layout.cpp \
    game.cpp \
    main.cpp \
    chess.cpp \
    notation_model.cpp \
    promotion_square.cpp \
    settings_dialog.cpp \
    square.cpp \
    timer.cpp

HEADERS += \
    board_color_square.h \
    chess.h \
    chess_layout.h \
    game.h \
    notation_model.h \
    piece.h \
    position.h \
    promotion_square.h \
    settings.h \
    settings_dialog.h \
    square.h \
    timer.h

FORMS += \
    chess.ui \
    settings_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
