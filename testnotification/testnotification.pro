TARGET = testnotification
TEMPLATE = lib
CONFIG   += plugin

# Qt Mobility 1.2
maemo5 {
	CONFIG += mobility12
} else {
	CONFIG += mobility
}
MOBILITY += systeminfo

SOURCES += testnotificationplugin.cpp testnotificationprovider.cpp \
    testnotification.cpp \
    testweathernotification.cpp

HEADERS += testnotificationplugin.h testnotificationprovider.h \
    testnotification.h \
    testweathernotification.h

unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/notifications
	} else {
		target.path = /usr/lib/sowatch/notifications
	}
	INSTALLS += target
}
