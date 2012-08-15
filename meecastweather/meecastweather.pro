TARGET   = meecastweather
TEMPLATE = lib
CONFIG   += plugin
QT       += xml

SOURCES += meecastplugin.cpp meecastprovider.cpp meecastweather.cpp

HEADERS += meecastplugin.h meecastprovider.h meecastweather.h

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
