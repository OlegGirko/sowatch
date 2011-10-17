#-------------------------------------------------
#
# Project created by QtCreator 2011-09-17T04:55:26
#
#-------------------------------------------------

TEMPLATE = app

TARGET = sowatchd

QT       += core gui
CONFIG   += mobility
CONFIG   -= app_bundle
MOBILITY += serviceframework

SOURCES += main.cpp \
    daemon.cpp \
    service.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -lsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -lsowatch
else:symbian: LIBS += -lsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/bin
	} else {
		target.path = /usr/bin
	}
	INSTALLS += target
}

HEADERS += \
    daemon.h \
    service.h \
    global.h

xml.path = /opt/sowatch/xml
xml.files = service.xml
dbus.path = /usr/share/dbus-1/services
dbus.files = com.javispedro.sowatch.service.sowatch-service.service
INSTALLS += xml dbus
