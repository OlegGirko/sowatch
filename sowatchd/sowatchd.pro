TARGET = sowatchd

TEMPLATE = app

QT       += core gui dbus
CONFIG   -= app_bundle

SOURCES += main.cpp daemon.cpp allscanner.cpp scanneradaptor.cpp daemonadaptor.cpp
HEADERS += global.h daemon.h allscanner.h scanneradaptor.h daemonadaptor.h

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

xml.files = service.xml
INSTALLS += xml

!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	QMAKE_RPATHDIR += /opt/sowatch/lib
	target.path = /opt/sowatch/bin
	xml.path = /opt/sowatch/xml
} else {
	target.path = /usr/bin
	xml.path = /usr/share/sowatch/xml
}
INSTALLS += target

dbus.path = /usr/share/dbus-1/services
dbus.files = com.javispedro.sowatch.service.sowatch-service.service
INSTALLS += dbus

OTHER_FILES += scanner.xml \
    daemon.xml
