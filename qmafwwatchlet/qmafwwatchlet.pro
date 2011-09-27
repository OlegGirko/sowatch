
TARGET = qmafwwatchlet
TEMPLATE = lib
# CONFIG   += plugin
QT       += dbus
CONFIG   += link_pkgconfig
unix: PKGCONFIG += qmafw qmafw-shared

SOURCES += qmafwwatchlet.cpp \
    qmafwwatchletplugin.cpp \
    qmafwwatchletplayer.cpp
HEADERS += qmafwwatchlet.h \
    qmafwwatchletplugin.h \
    qmafwwatchletplayer.h

unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	maemo5 {
		target.path = /opt/sowatch/watchlets
	} else {
		target.path = /usr/lib/sowatch/watchlets
	}
	INSTALLS += target
}

OTHER_FILES += \
    metawatch-digital.qml

RESOURCES += \
    qmafwwatchlet.qrc






