QT -= gui
QT += concurrent

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    engine/engine.cpp \
    engine/engine_game_state.cpp \
    engine/node.cpp

HEADERS += \
    engine/engine.h \
    engine/engine_game_state.h \
    engine/node.h

include(../common.pri)
include(../rules/rules.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    engine.pri
