QT += core

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11
CONFIG += debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    position/game_state.cpp \
    position/move.cpp \
    position/position.cpp \
    position/rules.cpp

HEADERS += \
    position/game_state.h \
    position/move.h \
    position/position.h \
    position/rules.h

include(../common.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
