#ifndef SOWATCH_GLOBAL_H
#define SOWATCH_GLOBAL_H

#include <QtCore/qglobal.h>
#include <qplatformdefs.h>

#if defined(SOWATCH_LIBRARY)
#	define SOWATCH_EXPORT Q_DECL_EXPORT
#else
#	define SOWATCH_EXPORT Q_DECL_IMPORT
#endif

#if defined(QT_SIMULATOR) || (!defined(MEEGO_VERSION_MAJOR) && !defined(Q_WS_MAEMO_5) && !defined(QT_NO_DEBUG))
#	define SOWATCH_PLUGINS_DIR      ".."
#	define SOWATCH_RESOURCES_DIR    ".."
#	define SOWATCH_QML_DIR          ".."
#	define SOWATCH_I18N_DIR         ".."
#elif defined(MEEGO_VERSION_MAJOR) || defined(Q_WS_MAEMO_5)
#	define SOWATCH_PLUGINS_DIR      "/opt/sowatch/lib"
#	define SOWATCH_RESOURCES_DIR    "/opt/sowatch/share"
#	define SOWATCH_QML_DIR          "/opt/sowatch/qml"
#	define SOWATCH_I18N_DIR         "/opt/sowatch/i18n"
#elif defined(Q_OS_LINUX)
#	define SOWATCH_PLUGINS_DIR      "/usr/lib/sowatch"
#	define SOWATCH_RESOURCES_DIR    "/usr/share/sowatch"
#	define SOWATCH_QML_DIR          SOWATCH_RESOURCES_DIR "/qml"
#	define SOWATCH_I18N_DIR         SOWATCH_RESOURCES_DIR "/i18n"
#else
#	error Please define the paths for your platform
#endif

#define SOWATCH_DRIVERS_DIR       SOWATCH_PLUGINS_DIR "/drivers"
#define SOWATCH_NOTIFICATIONS_DIR SOWATCH_PLUGINS_DIR "/notifications"
#define SOWATCH_WATCHLETS_DIR     SOWATCH_PLUGINS_DIR "/watchlets"

#endif // SOWATCH_GLOBAL_H
