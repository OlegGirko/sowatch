TARGET = metawatchdriver
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

SOURCES += metawatchplugin.cpp \
    metawatchsimulatorform.cpp \
    metawatchsimulator.cpp \
    metawatchpaintengine.cpp \
    metawatch.cpp \
    metawatchdigital.cpp \
    metawatchanalog.cpp \
    metawatchscanner.cpp

HEADERS += metawatchplugin.h \
    metawatchsimulatorform.h \
    metawatchsimulator.h \
    metawatchpaintengine.h \
    metawatch.h \
    metawatchdigital.h \
    metawatchanalog.h \
    metawatchscanner.h

FORMS += \
	metawatchsimulatorform.ui

res_files.files += res/graphics res/fonts
qml_files.files += qml/com

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

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
INSTALLS += target res_files qml_files
