
TARGET = meegohandsetnotification
TEMPLATE = lib
# CONFIG   += plugin
QT       += dbus

SOURCES += meegohandsetplugin.cpp \
    meegohandsetnotificationprovider.cpp \
    watchnotificationsink.cpp \
    mnotificationmanagerinterface.cpp \
    meegohandsetnotification.cpp

HEADERS += meegohandsetplugin.h \
    meegohandsetnotificationprovider.h \
    watchnotificationsink.h \
    mnotificationmanagerinterface.h \
    meegohandsetnotification.h

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










