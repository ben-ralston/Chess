TEMPLATE = lib
CONFIG += staticlib

include(../config.pri)

SOURCES += \
    rules/game_state.cpp \
    rules/move.cpp \
    rules/position.cpp

HEADERS += \
    rules/game_state.h \
    rules/move.h \
    rules/position.h

include(../common.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
