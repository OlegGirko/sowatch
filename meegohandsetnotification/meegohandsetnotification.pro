
TARGET = meegohandsetnotification
TEMPLATE = lib
CONFIG   += plugin
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
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/notifications
	} else {
		target.path = /usr/lib/sowatch/notifications
	}
	INSTALLS += target
}
