TARGET = liveviewwatchlets
TEMPLATE = lib
CONFIG   += plugin

SOURCES += liveviewwatchletsplugin.cpp \
	liveviewnotificationwatchlet.cpp

HEADERS += liveviewwatchletsplugin.h \
	liveviewnotificationwatchlet.h \
    liveviewnotificationwatchlet.h

qml_files.files = liveview-notification.qml

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
QML_IMPORT_PATH += $$PWD/../liveview/qml

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

OTHER_FILES += \
    liveview-notification.qml
