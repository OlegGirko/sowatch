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
    metawatch.cpp

HEADERS += metawatchplugin.h \
    metawatchsimulatorform.h \
    metawatchsimulator.h \
    metawatchpaintengine.h \
    metawatch.h

FORMS += \
	metawatchsimulatorform.ui

RESOURCES += \
	uires.qrc

OTHER_FILES += \
	idle_sms.bmp \
	idle_gmail.bmp \
	idle_call.bmp

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
    addFiles.sources = metawatch.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
		target.path = /opt/sowatch/drivers
    } else {
		target.path = /usr/lib/sowatch/drivers
    }
    INSTALLS += target
}
