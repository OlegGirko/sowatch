#-------------------------------------------------
#
# Project created by QtCreator 2011-09-24T00:00:03
#
#-------------------------------------------------

TARGET = sysinfowatchlet
TEMPLATE = lib
# CONFIG   += plugin # Qt creator doesn't want to deploy plugins
QT       += network
CONFIG   += mobility
MOBILITY += systeminfo

SOURCES += sysinfoplugin.cpp sysinfowatchlet.cpp

HEADERS += sysinfoplugin.h sysinfowatchlet.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -lsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -lsowatch
else:symbian: LIBS += -lsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/watchlets
	} else {
		target.path = /usr/lib/sowatch/watchlets
	}
	INSTALLS += target
}

OTHER_FILES += \
    metawatch-digital.qml

RESOURCES += \
    sysinfowatchlet.qrc
