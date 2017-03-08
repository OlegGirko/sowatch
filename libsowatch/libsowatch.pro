#-------------------------------------------------
#
# Project created by QtCreator 2011-09-16T23:45:06
#
#-------------------------------------------------

QT        += gui qml

CONFIG    += link_pkgconfig

packagesExist(dconf) {
    PKGCONFIG += dconf
    DEFCONFKEY = DConfKey
    DEFINES += USE_DCONF
} else:packagesExist(gconf-2.0) {
    PKGCONFIG += gconf-2.0
    DEFCONFKEY = GConfKey
    DEFINES += USE_GCONF
} else {
    error("One of these libraries is requires: dconf, gconf")
}

TARGET    = sowatch
TEMPLATE  = lib
VERSION   = 1.0.0

DEFINES += SOWATCH_LIBRARY

defconfkey.target = $${OUT_PWD}/defconfkey.h
defconfkey.commands = echo "'$${LITERAL_HASH}ifndef SOWATCH_DEFCONFKEY_H'" > $${OUT_PWD}/defconfkey.h;
defconfkey.commands += echo "'$${LITERAL_HASH}define SOWATCH_DEFCONFKEY_H'" >> $${OUT_PWD}/defconfkey.h;
defconfkey.commands += echo "'$${LITERAL_HASH}include \"$$lower($$DEFCONFKEY).h\"'" >> $${OUT_PWD}/defconfkey.h;
defconfkey.commands += echo "'namespace sowatch { typedef $$DEFCONFKEY DefConfKey; }'" >> $${OUT_PWD}/defconfkey.h;
defconfkey.commands += echo "'$${LITERAL_HASH}endif // SOWATCH_DEFCONFKEY_H'" >> $${OUT_PWD}/defconfkey.h

QMAKE_EXTRA_TARGETS += defconfkey
INCLUDEPATH += $$OUT_PWD
DEPENDPATH += $$OUT_PWD
HEADERS += $${OUT_PWD}/defconfkey.h
PRE_TARGETDEPS += $${OUT_PWD}/defconfkey.h

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
    $$lower($$DEFCONFKEY).cpp \
    notificationsmodel.cpp \
    watchletsmodel.cpp

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
    $$lower($$DEFCONFKEY).h \
    notificationsmodel.h \
    watchletsmodel.h

TRANSLATIONS += libsowatch_en.ts libsowatch_es.ts

install_headers.files = $$HEADERS
install_translations.files = libsowatch*.qm

install_headers.path = /usr/include/sowatch
!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	target.path = /opt/sowatch/lib
	install_translations.path = /opt/sowatch/i18n
} else {
	DEFINES += LIBDIR=\"\\\"$$[QT_INSTALL_LIBS]\\\"\"
	target.path = $$[QT_INSTALL_LIBS]
	install_translations.path = /usr/share/sowatch/i18n
}

INSTALLS += install_headers install_translations target
