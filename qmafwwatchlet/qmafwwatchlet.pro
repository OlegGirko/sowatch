
TARGET = qmafwwatchlet
TEMPLATE = lib
CONFIG   += plugin
QT       += dbus
CONFIG   += link_pkgconfig
PKGCONFIG += qmafw qmafw-shared

SOURCES += qmafwwatchlet.cpp \
    qmafwwatchletplugin.cpp \
    qmafwwatchletplayer.cpp
HEADERS += qmafwwatchlet.h \
    qmafwwatchletplugin.h \
    qmafwwatchletplayer.h

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/watchlets
	} else {
		target.path = /usr/lib/sowatch/watchlets
	}
	INSTALLS += target
}

OTHER_FILES += \
    metawatch-digital.qml

RESOURCES += \
    qmafwwatchlet.qrc
