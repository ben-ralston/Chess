TEMPLATE = lib
CONFIG += staticlib

include(../config.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
else: QT += gui

SOURCES += \
    settings/board_color_square.cpp \
    settings/settings_dialog.cpp

HEADERS += \
    settings/board_color_square.h \
    settings/settings.h \
    settings/settings_dialog.h

FORMS += \
    settings/settings_dialog.ui

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
