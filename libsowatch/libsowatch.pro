#-------------------------------------------------
#
# Project created by QtCreator 2011-09-16T23:45:06
#
#-------------------------------------------------

QT       += gui dbus declarative
CONFIG   += mobility
MOBILITY += connectivity

TARGET = libsowatch
TEMPLATE = lib

DEFINES += SOWATCH_LIBRARY

SOURCES += \
    watchsimulator.cpp \
    watchserver.cpp \
    watchpaintengine.cpp \
    watchlet.cpp \
    watch.cpp \
    testwatchlet.cpp \
    testdeclarativewatchlet.cpp \
    graphicswatchlet.cpp \
    declarativewatchwrapper.cpp \
    declarativewatchlet.cpp \
    watchplugininterface.cpp

HEADERS +=\
    watchsimulator.h \
    watchserver.h \
    watchpaintengine.h \
    watchlet.h \
    watch.h \
    testwatchlet.h \
    testdeclarativewatchlet.h \
    sowatch.h \
    graphicswatchlet.h \
    declarativewatchwrapper.h \
    declarativewatchlet.h \
    sowatch_global.h \
    watchplugininterface.h

FORMS +=

install_headers.files =\
	watchsimulator.h \
	watchserver.h \
	watchpaintengine.h \
	watchmanager.h \
	watchlet.h \
	watch.h \
	testwatchlet.h \
	testdeclarativewatchlet.h \
	sowatch.h \
	graphicswatchlet.h \
	declarativewatchwrapper.h \
	declarativewatchlet.h \
	sowatch_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE6B95AFF
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = libsowatch.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
	install_headers.path = /usr/include/sowatch
    maemo5 {
		target.path = /opt/maemo/usr/lib
    } else {
        target.path = /usr/lib
    }
	INSTALLS += install_headers target
}







