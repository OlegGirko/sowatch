
TARGET = qmapwatchlet
TEMPLATE = lib
CONFIG   += plugin
CONFIG   += mobility
MOBILITY += location

SOURCES += qmapwatchletplugin.cpp qmapwatchlet.cpp mapview.cpp \
    compasswatchlet.cpp

HEADERS += qmapwatchletplugin.h qmapwatchlet.h mapview.h \
    compasswatchlet.h

qml_files.files = map-metawatch-digital.qml map-icon.png map-arrow.png \
	compass-metawatch-digital.qml compass-icon.png

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
