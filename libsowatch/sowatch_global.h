#ifndef SOWATCH_GLOBAL_H
#define SOWATCH_GLOBAL_H

#include <QtCore/qglobal.h>
#include <qplatformdefs.h>

#if defined(SOWATCH_LIBRARY)
#	define SOWATCH_EXPORT Q_DECL_EXPORT
#else
#	define SOWATCH_EXPORT Q_DECL_IMPORT
#endif

#if defined(MEEGO_VERSION_MAJOR) || defined(Q_WS_MAEMO_5)
#	define SOWATCH_PLUGINS_DIR "/opt/sowatch/lib"
#elif defined(Q_OS_LINUX)
#	define SOWATCH_PLUGINS_DIR "/usr/lib/sowatch"
#else
#	error Please define a path for your platform
#endif

#define SOWATCH_DRIVERS_DIR       SOWATCH_PLUGINS_DIR "/drivers"
#define SOWATCH_NOTIFICATIONS_DIR SOWATCH_PLUGINS_DIR "/notifications"
#define SOWATCH_WATCHLETS_DIR     SOWATCH_PLUGINS_DIR "/watchlets"

#endif // SOWATCH_GLOBAL_H
