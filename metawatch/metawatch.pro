TARGET = metawatchdriver
TEMPLATE = lib
CONFIG   += plugin
QT       += gui bluetooth widgets

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
    metawatchdigitalsimulatorform.cpp

HEADERS += metawatchplugin.h \
    metawatchpaintengine.h \
    metawatch.h \
    metawatchdigital.h \
    metawatchanalog.h \
    metawatchscanner.h \
    metawatchdigitalsimulator.h \
    metawatchdigitalsimulatorform.h

FORMS += \
    metawatchdigitalsimulatorform.ui

OTHER_FILES += \
    metawatch.json

res_files.files += res/graphics res/fonts
qml_files.files += qml/com qml/metawatch-digital-config.qml

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch $$OUT_PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch $$OUT_PWD/../libsowatch

LIBS += -L$$OUT_PWD/../libsowatchbt/ -lsowatchbt
INCLUDEPATH += $$PWD/../libsowatchbt
DEPENDPATH += $$PWD/../libsowatchbt

!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	QMAKE_RPATHDIR += /opt/sowatch/lib
	target.path = /opt/sowatch/lib/drivers
	res_files.path = /opt/sowatch/share/metawatch
	qml_files.path = /opt/sowatch/qml
} else {
	target.path = $$[QT_INSTALL_LIBS]/sowatch/drivers
	res_files.path = /usr/share/sowatch/metawatch
	qml_files.path = /usr/share/sowatch/qml
}
INSTALLS += target res_files qml_files
