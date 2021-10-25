TEMPLATE = app

include(../config.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
else: QT += gui

SOURCES += \
    application/application_game_state.cpp \
    application/chess.cpp \
    application/chess_layout.cpp \
    application/game.cpp \
    application/main.cpp \
    application/notation_model.cpp \
    application/promotion_square.cpp \
    application/square.cpp \
    application/timer.cpp

HEADERS += \
    application/application_game_state.h \
    application/chess.h \
    application/chess_layout.h \
    application/game.h \
    application/notation_model.h \
    application/promotion_square.h \
    application/square.h \
    application/timer.h

FORMS += \
    application/chess.ui

include(../common.pri)
include(../settings/settings.pri)
include(../rules/rules.pri)
include(../engine/engine.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
