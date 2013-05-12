TARGET = metawatchwatchlets
TEMPLATE = lib
CONFIG   += plugin

SOURCES += metawatchwatchletsplugin.cpp \
	metawatchfacewatchlet.cpp metawatchnotificationwatchlet.cpp

HEADERS += metawatchwatchletsplugin.h \
	metawatchfacewatchlet.h metawatchnotificationwatchlet.h

qml_files.files = metawatch-digital-watchface.qml metawatch-digital-notification.qml

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
		target.path = /usr/lib/sowatch/watchlets
		qml_files.path = /usr/share/sowatch/qml/$$TARGET
	}
	INSTALLS += target qml_files
}

OTHER_FILES += \
    ChatBubble.qml
