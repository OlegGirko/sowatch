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
    metawatchpaintengine.cpp \
    metawatch.cpp \
    metawatchdigital.cpp \
    metawatchanalog.cpp \
    metawatchscanner.cpp \
    metawatchdigitalsimulator.cpp \
    metawatchdigitalsimulatorform.cpp \
    metawatchdigitalfacewatchlet.cpp

HEADERS += metawatchplugin.h \
    metawatchpaintengine.h \
    metawatch.h \
    metawatchdigital.h \
    metawatchanalog.h \
    metawatchscanner.h \
    metawatchdigitalsimulator.h \
    metawatchdigitalsimulatorform.h \
    metawatchdigitalfacewatchlet.h

FORMS += \
    metawatchdigitalsimulatorform.ui

res_files.files += res/graphics res/fonts
qml_files.files += qml/com qml/metawatch-digital-config.qml

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

OTHER_FILES += \
    qml/metawatch-digital-watchface.qml
