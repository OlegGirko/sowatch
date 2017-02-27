
TARGET = notificationswatchlet
TEMPLATE = lib
CONFIG   += plugin

QT += widgets

SOURCES += notificationswatchletplugin.cpp notificationswatchlet.cpp

HEADERS += notificationswatchletplugin.h notificationswatchlet.h

OTHER_FILES += \
    notificationswatchlets.json

qml_files.files = metawatch-digital.qml icon.png

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -lsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -lsowatch
else:symbian: LIBS += -lsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

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
