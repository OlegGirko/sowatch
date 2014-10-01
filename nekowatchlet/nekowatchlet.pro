TARGET = nekowatchlet
TEMPLATE = lib
CONFIG   += plugin

SOURCES += nekowatchletplugin.cpp nekowatchlet.cpp

HEADERS += nekowatchletplugin.h nekowatchlet.h

qml_files.files = metawatch-digital.qml liveview.qml \
	icon.png metawatch-digital-icon.png liveview-icon.png \
	Neko.qml neko.png neko-inv.png

LIBS += -Wl,-L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
QML_IMPORT_PATH += $$PWD/../metawatch/qml

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
