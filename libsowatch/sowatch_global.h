#ifndef SOWATCH_GLOBAL_H
#define SOWATCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SOWATCH_LIBRARY)
#  define SOWATCH_EXPORT Q_DECL_EXPORT
#else
#  define SOWATCH_EXPORT Q_DECL_IMPORT
#endif

#endif // SOWATCH_GLOBAL_H
