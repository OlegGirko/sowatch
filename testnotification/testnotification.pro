TARGET = testnotification
TEMPLATE = lib
CONFIG   += plugin
CONFIG += mobility
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
		target.path = $$[QT_INSTALL_LIBS]/sowatch/notifications
	}
	INSTALLS += target
}
