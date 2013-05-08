TARGET = liveviewdriver
TEMPLATE = lib
CONFIG   += plugin
QT       += gui

# Qt Mobility 1.2
maemo5 {
	CONFIG += mobility12
} else {
	CONFIG += mobility
}
MOBILITY += connectivity systeminfo

SOURCES += liveviewplugin.cpp \
    liveviewscanner.cpp \
    liveview.cpp
HEADERS += liveviewplugin.h \
    liveviewscanner.h \
    liveview.h

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

LIBS += -L$$OUT_PWD/../libsowatchbt/ -lsowatchbt
INCLUDEPATH += $$PWD/../libsowatchbt
DEPENDPATH += $$PWD/../libsowatchbt

!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	QMAKE_RPATHDIR += /opt/sowatch/lib
	target.path = /opt/sowatch/lib/drivers
	res_files.path = /opt/sowatch/share/metawatch
	qml_files.path = /opt/sowatch/qml
} else {
	target.path = /usr/lib/sowatch/drivers
	res_files.path = /usr/share/sowatch/metawatch
	qml_files.path = /usr/share/sowatch/qml
}
INSTALLS += target
