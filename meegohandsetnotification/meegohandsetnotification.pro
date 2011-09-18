#-------------------------------------------------
#
# Project created by QtCreator 2011-09-18T18:48:13
#
#-------------------------------------------------


TARGET = meegohandsetnotification
TEMPLATE = lib
# CONFIG   += plugin # Stupid Qt creator doesn't want to deploy plugins
QT       += dbus

SOURCES += meegohandsetplugin.cpp \
    meegohandsetnotificationprovider.cpp \
    watchnotificationsink.cpp \
    mnotificationmanagerinterface.cpp

HEADERS += meegohandsetplugin.h \
    meegohandsetnotificationprovider.h \
    watchnotificationsink.h \
    mnotificationmanagerinterface.h

CONFIG += notificationsystem

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








