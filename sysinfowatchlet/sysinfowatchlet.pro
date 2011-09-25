#-------------------------------------------------
#
# Project created by QtCreator 2011-09-24T00:00:03
#
#-------------------------------------------------

TARGET = sysinfowatchlet
TEMPLATE = lib
# CONFIG   += plugin # Stupid Qt creator doesn't want to deploy plugins
QT       += network
CONFIG   += mobility
MOBILITY += systeminfo

SOURCES += sysinfoplugin.cpp sysinfowatchlet.cpp

HEADERS += sysinfoplugin.h sysinfowatchlet.h

unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	maemo5 {
		target.path = /opt/sowatch/watchlets
	} else {
		target.path = /usr/lib/sowatch/watchlets
	}
	INSTALLS += target
}

OTHER_FILES += \
    metawatch-digital.qml

RESOURCES += \
    sysinfowatchlet.qrc








