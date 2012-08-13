#-------------------------------------------------
#
# Project created by QtCreator 2011-09-16T23:45:06
#
#-------------------------------------------------

QT        += gui declarative

CONFIG    += link_pkgconfig
PKGCONFIG += gconf-2.0

TARGET    = sowatch
TEMPLATE  = lib
VERSION   = 1.0.0

DEFINES += SOWATCH_LIBRARY

SOURCES += \
    watchserver.cpp \
    watchpaintengine.cpp \
    watchlet.cpp \
    watch.cpp \
    graphicswatchlet.cpp \
    declarativewatchwrapper.cpp \
    declarativewatchlet.cpp \
    watchplugininterface.cpp \
    notification.cpp \
	weathernotification.cpp \
    notificationplugininterface.cpp \
    notificationprovider.cpp \
    watchletplugininterface.cpp \
	registry.cpp \
    watchscanner.cpp \
    allwatchscanner.cpp \
    configkey.cpp \
    gconfkey.cpp \
    notificationsmodel.cpp

HEADERS += \
    watchserver.h \
    watchpaintengine.h \
    watchlet.h \
    watch.h \
    sowatch.h \
    graphicswatchlet.h \
    declarativewatchwrapper.h \
    declarativewatchlet.h \
    sowatch_global.h \
    watchplugininterface.h \
    notification.h \
	weathernotification.h \
    notificationplugininterface.h \
    notificationprovider.h \
    watchletplugininterface.h \
	registry.h \
    watchscanner.h \
    allwatchscanner.h \
    configkey.h \
    gconfkey.h \
    notificationsmodel.h

install_headers.files = $$HEADERS

install_headers.path = /usr/include/sowatch
!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	target.path = /opt/sowatch/lib
} else {
	target.path = /usr/lib
}

INSTALLS += install_headers target
