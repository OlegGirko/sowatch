TARGET = sowatch

QT += dbus

# Add more folders to ship with the application, here
qml_folder.source = qml
qml_folder.target = .
DEPLOYMENTFOLDERS = qml_folder

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Dependency to the main Sowatch library
LIBS += -L$$OUT_PWD/../libsowatch/ -lsowatch
INCLUDEPATH += $$PWD/../libsowatch
DEPENDPATH += $$PWD/../libsowatch

# Source files
SOURCES += main.cpp

OTHER_FILES += \
	qml/MainPage.qml \
	qml/main.qml \
    sowatch_harmattan.desktop \
    sowatch.desktop

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
