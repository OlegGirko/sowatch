#-------------------------------------------------
#
# Project created by QtCreator 2011-09-17T03:22:32
#
#-------------------------------------------------

TARGET = metawatch
TEMPLATE = lib
CONFIG   += mobility
MOBILITY += connectivity

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
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/release/ -llibsowatch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libsowatch/debug/ -llibsowatch
else:symbian: LIBS += -llibsowatch
else:unix: LIBS += -L$$OUT_PWD/../libsowatch/ -llibsowatch

INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
