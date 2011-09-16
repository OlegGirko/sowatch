#-------------------------------------------------
#
# Project created by QtCreator 2011-09-12T20:48:51
#
#-------------------------------------------------

QT       += core gui declarative

CONFIG += mobility
MOBILITY += connectivity

TARGET = sowatch
TEMPLATE = app


SOURCES += main.cpp\
    watchmanager.cpp \
    watchserver.cpp \
    watch.cpp \
    watchsimulator.cpp \
    metawatchsimulatorform.cpp \
    metawatchsimulator.cpp \
    watchlet.cpp \
    testwatchlet.cpp \
    watchpaintengine.cpp \
    graphicswatchlet.cpp \
    declarativewatchlet.cpp \
    testdeclarativewatchlet.cpp \
    declarativewatchwrapper.cpp \
    metawatch.cpp \
    metawatchpaintengine.cpp

HEADERS  += \
    watchmanager.h \
    watchserver.h \
    watch.h \
    watchsimulator.h \
    metawatchsimulatorform.h \
    metawatchsimulator.h \
    watchlet.h \
    testwatchlet.h \
    watchpaintengine.h \
    graphicswatchlet.h \
    declarativewatchlet.h \
    testdeclarativewatchlet.h \
    declarativewatchwrapper.h \
    metawatch.h \
    sowatch.h \
    metawatchpaintengine.h

FORMS    += \
    metawatchsimulatorform.ui

OTHER_FILES += \
    testdeclarativewatchlet.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qt.png

RESOURCES += \
    tests.qrc

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/sowatch/bin
    INSTALLS += target
}






