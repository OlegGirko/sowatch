#-------------------------------------------------
#
# Project created by QtCreator 2011-09-24T00:00:03
#
#-------------------------------------------------

TARGET = ckitcallnotification
TEMPLATE = lib
# CONFIG   += plugin # Stupid Qt creator doesn't want to deploy plugins
QT       += dbus

SOURCES += ckitcallplugin.cpp \
	ckitcallprovider.cpp \
    ckitcallnotification.cpp

HEADERS += ckitcallplugin.h \
	ckitcallprovider.h \
    ckitcallnotification.h

CONFIG += link_pkgconfig
PKGCONFIG += contextsubscriber-1.0

unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	maemo5 {
		target.path = /opt/sowatch/notifications
	} else {
		target.path = /usr/lib/sowatch/notifications
	}
	INSTALLS += target
}






