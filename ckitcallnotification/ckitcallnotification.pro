#-------------------------------------------------
#
# Project created by QtCreator 2011-09-24T00:00:03
#
#-------------------------------------------------

TARGET = ckitcallnotification
TEMPLATE = lib
CONFIG   += plugin
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
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/notifications
	} else {
		target.path = $$[QT_INSTALL_LIBS]/sowatch/notifications
	}
	INSTALLS += target
}
