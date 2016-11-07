#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T15:18:41
#
#-------------------------------------------------

QT       -= gui

TARGET = SmartCardIO
TEMPLATE = lib

DEFINES += QSMARTCARDIO_LIBRARY

SOURCES += \
    APDUCommand.cpp \
    APDUResponse.cpp \
    cardevent.cpp \
    NativeSCard.cpp \
    scardexception.cpp

HEADERS +=\
    APDUCommand.h \
    APDUResponse.h \
    cardevent.h \
    NativeSCard.h \
    scardexception.h \
    winscarderr.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -lwinscard
