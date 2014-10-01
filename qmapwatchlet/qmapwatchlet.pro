
TARGET = qmapwatchlet
TEMPLATE = lib
CONFIG   += plugin

# Qt Mobility 1.2
maemo5 {
	CONFIG += mobility12
} else {
	CONFIG += mobility
}
MOBILITY += location

SOURCES += qmapwatchletplugin.cpp qmapwatchlet.cpp mapview.cpp \
    compasswatchlet.cpp compassview.cpp

HEADERS += qmapwatchletplugin.h qmapwatchlet.h mapview.h \
    compasswatchlet.h compassview.h

qml_files.files = map-metawatch-digital.qml map-liveview.qml \
	map-icon.png map-metawatch-digital-icon.png map-liveview-icon.png arrow.png \
	compass-metawatch-digital.qml compass-liveview.qml \
	compass-icon.png compass-metawatch-digital-icon.png compass-liveview-icon.png compass.png

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
