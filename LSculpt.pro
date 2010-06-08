# -------------------------------------------------
# Project created by QtCreator 2010-04-22T21:11:11
# -------------------------------------------------
# QT += opengl
TARGET = LSculpt
TEMPLATE = app
LIBS += -L./ \
    -lLDVLib

# Comment this out to build UI version of LSculpt
# CONFIG += console
console:DEFINES += LSCULPT_CONSOLE
SOURCES += main.cpp \
    LSculpt.cpp \
    rply.c \
    lsculptmainwin.cpp \
    argpanel.cpp \
    obj_parser.cpp
HEADERS += rply.h \
    LSculpt_functions.h \
    LSculpt_classes.h \
    smVector.h \
    lsculptmainwin.h \
    argpanel.h \
    obj_parser.hpp \
    obj.hpp \
    obj_config.hpp \
    LSculpt_obj.h \
    LDVLib.h
FORMS += lsculptmainwin.ui \
    argpanel.ui \
    advanced_widgets.ui
RC_FILE = LSculpt.rc
