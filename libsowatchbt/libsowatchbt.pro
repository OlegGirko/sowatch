TARGET    = sowatchbt
TEMPLATE  = lib
VERSION   = 1.0.0

# Qt Mobility 1.2
maemo5 {
	CONFIG += mobility12
} else {
	CONFIG += mobility
}
MOBILITY += connectivity systeminfo

QT += bluetooth widgets

DEFINES += SOWATCHBT_LIBRARY

SOURCES += \
    bluetoothwatch.cpp \
    bluetoothwatchscanner.cpp

HEADERS += sowatchbt.h sowatchbt_global.h \
    bluetoothwatch.h \
    bluetoothwatchscanner.h

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

install_headers.files = $$HEADERS

install_headers.path = /usr/include/sowatch
!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	target.path = /opt/sowatch/lib
	install_translations.path = /opt/sowatch/i18n
} else {
	target.path = /usr/lib
	install_translations.path = /usr/share/sowatch/i18n
}

INSTALLS += install_headers target
