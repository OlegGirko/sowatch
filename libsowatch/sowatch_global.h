#ifndef SOWATCH_GLOBAL_H
#define SOWATCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SOWATCH_LIBRARY)
#  define SOWATCH_EXPORT Q_DECL_EXPORT
#else
#  define SOWATCH_EXPORT Q_DECL_IMPORT
#endif

#if defined(Q_WS_MAEMO_5)
#  define SOWATCH_DRIVERS_DIR "/opt/sowatch/drivers"
#  define SOWATCH_NOTIFICATIONS_DIR "/opt/sowatch/notifications"
#  define SOWATCH_WATCHLETS_DIR "/opt/sowatch/watchlets"
#elif defined(Q_OS_LINUX)
#  define SOWATCH_DRIVERS_DIR "/usr/lib/sowatch/drivers"
#  define SOWATCH_NOTIFICATIONS_DIR "/usr/lib/sowatch/notifications"
#  define SOWATCH_WATCHLETS_DIR "/usr/lib/sowatch/watchlets"
#endif

#endif // SOWATCH_GLOBAL_H
