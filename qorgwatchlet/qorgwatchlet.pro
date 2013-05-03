TARGET = qorgwatchlet
TEMPLATE = lib
CONFIG   += plugin
CONFIG   += mobility
MOBILITY += organizer

SOURCES += qorgwatchletplugin.cpp qorgwatchlet.cpp

HEADERS += qorgwatchletplugin.h qorgwatchlet.h

qml_files.files = metawatch-digital.qml icon.png

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/watchlets
		qml_files.path = /opt/sowatch/qml/$$TARGET
	} else {
		target.path = /usr/lib/sowatch/watchlets
		qml_files.path = /usr/share/sowatch/qml/$$TARGET
	}
	INSTALLS += target qml_files
}
