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
	maemo5 {
		target.path = /opt/sowatch/notifications
	} else {
		target.path = /usr/lib/sowatch/notifications
	}
	INSTALLS += target
}








