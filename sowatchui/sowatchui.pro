TARGET = sowatch

QT += dbus

# Add more folders to ship with the application, here
qml_folder.source = qml
qml_folder.target = .
DEPLOYMENTFOLDERS = qml_folder

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
simulator {
	QML_IMPORT_PATH += $$[QT_INSTALL_PREFIX]/imports/simulatorHarmattan
	DEFINES += QT_INSTALL_PREFIX=\\\"$$[QT_INSTALL_PREFIX]\\\"
}

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Dependency to the main Sowatch library
LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch
!isEmpty(MEEGO_VERSION_MAJOR)|maemo5 {
	QMAKE_RPATHDIR += /opt/sowatch/lib
}

# Source files
SOURCES += main.cpp \
    watchesmodel.cpp daemonproxy.cpp \
    scanwatchesmodel.cpp scannerproxy.cpp

HEADERS += \
    watchesmodel.h daemonproxy.h \
    scanwatchesmodel.h scannerproxy.h

OTHER_FILES += qml/main.qml \
	qml/MainPage.qml \
	qml/NewWatchSheet.qml \
	qml/WatchPage.qml \
    sowatch_harmattan.desktop \
    sowatch.desktop

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
