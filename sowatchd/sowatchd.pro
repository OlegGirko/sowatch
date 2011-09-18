#-------------------------------------------------
#
# Project created by QtCreator 2011-09-17T04:55:26
#
#-------------------------------------------------

TEMPLATE = app

TARGET = sowatchd

QT       += core gui
CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp \
    daemon.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -lsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -lsowatch
else:symbian: LIBS += -lsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix {
	maemo5 {
		target.path = /opt/sowatch
	} else {
		target.path = /usr/bin
	}
	INSTALLS += target
}

HEADERS += \
    daemon.h


