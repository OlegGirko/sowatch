TARGET = sysinfowatchlet
TEMPLATE = lib
CONFIG   += plugin
QT       += network

# Qt Mobility 1.2
maemo5 {
	CONFIG += mobility12
} else {
	CONFIG += mobility
}
MOBILITY += systeminfo

SOURCES += sysinfoplugin.cpp sysinfowatchlet.cpp

HEADERS += sysinfoplugin.h sysinfowatchlet.h

qml_files.files = metawatch-digital.qml liveview.qml icon.png metawatch-digital-icon.png liveview-icon.png

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
QML_IMPORT_PATH += $$PWD/../metawatch/qml

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/watchlets
		qml_files.path = /opt/sowatch/qml/$$TARGET
	} else {
		target.path = $$[QT_INSTALL_LIBS]/sowatch/watchlets
		qml_files.path = /usr/share/sowatch/qml/$$TARGET
	}
	INSTALLS += target qml_files
}
