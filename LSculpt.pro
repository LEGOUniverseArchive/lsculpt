# -------------------------------------------------
# Project created by QtCreator 2010-04-22T21:11:11
# -------------------------------------------------
# QT += opengl
QT += widgets \
    core
TARGET = LSculpt
TEMPLATE = app

# Comment this out to build UI version of LSculpt
# CONFIG += console

console:DEFINES += LSCULPT_CONSOLE
# QMAKE_LFLAGS += -static-libgcc
SOURCES += LSculpt.cpp \
    rply.c \
    obj_parser.cpp
HEADERS += rply.h \
    LSculpt_functions.h \
    LSculpt_classes.h \
    smVector.h \
    obj_parser.hpp \
    obj.hpp \
    obj_config.hpp \
    LSculpt_obj.h
!console { 
    win32 {
        LIBS += -L./ \
            -lLDVLib
    }
    SOURCES += main.cpp \
        lsculptmainwin.cpp \
        argpanel.cpp \
        aboutdialog.cpp
    HEADERS += lsculptmainwin.h \
        argpanel.h \
        aboutdialog.h \
        LDVLib.h
    FORMS += lsculptmainwin.ui \
        argpanel.ui \
        aboutdialog.ui \
        advanced_widgets.ui
    RC_FILE = LSculpt.rc
    RESOURCES += LSculpt.qrc
}

