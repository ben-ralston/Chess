TEMPLATE = lib
CONFIG += staticlib

include(../config.pri)

QT += concurrent

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
