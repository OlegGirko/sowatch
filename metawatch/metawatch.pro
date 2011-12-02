#-------------------------------------------------
#
# Project created by QtCreator 2011-09-17T03:22:32
#
#-------------------------------------------------

TARGET = metawatchdriver
TEMPLATE = lib
# CONFIG   += plugin # Stupid Qt creator doesn't want to deploy plugins
QT       += gui
CONFIG   += mobility
MOBILITY += connectivity systeminfo

SOURCES += metawatchplugin.cpp \
    metawatchsimulatorform.cpp \
    metawatchsimulator.cpp \
    metawatchpaintengine.cpp \
    metawatch.cpp \
    metawatchdigital.cpp \
    metawatchanalog.cpp

HEADERS += metawatchplugin.h \
    metawatchsimulatorform.h \
    metawatchsimulator.h \
    metawatchpaintengine.h \
    metawatch.h \
    metawatchdigital.h \
    metawatchanalog.h

FORMS += \
	metawatchsimulatorform.ui

res_files.files += res/graphics res/fonts
qml_files.files += qml/MWListView.qml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -lsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -lsowatch
else:symbian: LIBS += -lsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4DC26B0
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
	addFiles.sources = metawatchdriver.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/drivers
		res_files.path = /opt/sowatch/share/metawatch
		qml_files.path = /opt/sowatch/qml/metawatch
    } else {
		target.path = /usr/lib/sowatch/drivers
		res_files.path = /usr/share/sowatch/metawatch
		qml_files.path = /usr/share/sowatch/qml/metawatch
    }
	INSTALLS += target res_files qml_files
}

OTHER_FILES += \
    qml/MWTitle.qml


