#ifndef SOWATCHBT_GLOBAL_H
#define SOWATCHBT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SOWATCHBT_LIBRARY)
#	define SOWATCHBT_EXPORT Q_DECL_EXPORT
#else
#	define SOWATCHBT_EXPORT Q_DECL_IMPORT
#endif

#endif // SOWATCHBT_GLOBAL_H
