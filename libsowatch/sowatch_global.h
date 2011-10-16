#ifndef SOWATCH_GLOBAL_H
#define SOWATCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SOWATCH_LIBRARY)
#  define SOWATCH_EXPORT Q_DECL_EXPORT
#else
#  define SOWATCH_EXPORT Q_DECL_IMPORT
#endif

#if defined(Q_WS_MAEMO_5)
#  define SOWATCH_PLUGINS_DIR "/opt/sowatch"
#elif defined(Q_OS_LINUX)
#  define SOWATCH_PLUGINS_DIR "/usr/lib/sowatch"
#endif

#define SOWATCH_DRIVERS_DIR       SOWATCH_PLUGINS_DIR "/drivers"
#define SOWATCH_NOTIFICATIONS_DIR SOWATCH_PLUGINS_DIR "/notifications"
#define SOWATCH_WATCHLETS_DIR     SOWATCH_PLUGINS_DIR "/watchlets"

#endif // SOWATCH_GLOBAL_H
