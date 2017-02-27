
TARGET = qmafwwatchlet
TEMPLATE = lib
CONFIG   += plugin
QT       += dbus
CONFIG   += link_pkgconfig
PKGCONFIG += dbus-glib-1 qmafw qmafw-shared

SOURCES += qmafwwatchlet.cpp \
    qmafwwatchletplugin.cpp \
    qmafwwatchletplayer.cpp \
    qmafwwatchletvolumecontrol.cc
HEADERS += qmafwwatchlet.h \
    qmafwwatchletplugin.h \
    qmafwwatchletplayer.h \
    qmafwwatchletvolumecontrol.h

qml_files.files = icon.png volume.png \
	metawatch-digital.qml metawatch-digital-icon.png \
	liveview.qml liveview-icon.png

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
