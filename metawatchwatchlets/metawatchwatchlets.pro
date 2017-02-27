TARGET = metawatchwatchlets
TEMPLATE = lib
CONFIG   += plugin

QT += gui widgets

SOURCES += metawatchwatchletsplugin.cpp \
	metawatchfacewatchlet.cpp metawatchnotificationwatchlet.cpp

HEADERS += metawatchwatchletsplugin.h \
	metawatchfacewatchlet.h metawatchnotificationwatchlet.h

OTHER_FILES += \
    metawatchwatchlets.json

qml_files.files = metawatch-digital-watchface.qml metawatch-digital-notification.qml ChatBubble.qml \
	bubble.png bubble_tip.png idle-border.png idle-call.png idle-mail.png idle-msg.png \
	notification-email.png notification-message.png notification-phone.png notification-timer.png \
	weather-cloudy.png weather-rain.png weather-snow.png weather-sunny.png weather-thunderstorm.png weather-wind.png

LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
QML_IMPORT_PATH += $$PWD/../metawatch/qml

unix:!symbian {
	!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
		QMAKE_RPATHDIR += /opt/sowatch/lib
		target.path = /opt/sowatch/lib/watchlets
		qml_files.path = /opt/sowatch/qml/$$TARGET
	} else {
		target.path = $$[QT_INSTALL_LIBS]/sowatch/watchlets
		qml_files.path = /usr/share/sowatch/qml/$$TARGET
	}
	INSTALLS += target qml_files
}
