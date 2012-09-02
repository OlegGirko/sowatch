TARGET = sowatchd

TEMPLATE = app

QT       += core gui dbus
CONFIG   -= app_bundle

SOURCES += main.cpp daemon.cpp daemonadaptor.cpp watchhandler.cpp
HEADERS += daemon.h daemonadaptor.h watchhandler.h

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

# Library & target paths
!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	QMAKE_RPATHDIR += /opt/sowatch/lib
	target.path = /opt/sowatch/bin
} else {
	target.path = /usr/bin
}
INSTALLS += target

# Service files
!isEmpty(MEEGO_VERSION_MAJOR) {
	service.files = sowatchd.conf
	service.path = /etc/init/apps
	INSTALLS += service
}

dbus.path = /usr/share/dbus-1/services
dbus.files = com.javispedro.sowatch.service.sowatch-service.service
INSTALLS += dbus

OTHER_FILES += daemon.xml
