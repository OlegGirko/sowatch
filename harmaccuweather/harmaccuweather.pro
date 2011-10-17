TARGET = harmaccuweather
TEMPLATE = lib
# CONFIG   += plugin # Qt creator doesn't want to deploy plugins
CONFIG += mobility
MOBILITY += systeminfo

SOURCES += harmaccuplugin.cpp harmaccuprovider.cpp \
    harmaccuweather.cpp

HEADERS += harmaccuplugin.h harmaccuprovider.h \
    harmaccuweather.h

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
