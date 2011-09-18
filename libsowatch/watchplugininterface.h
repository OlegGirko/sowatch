#ifndef WATCHPLUGININTERFACE_H
#define WATCHPLUGININTERFACE_H

#include <QtPlugin>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include "sowatch_global.h"

namespace sowatch
{

class Watch;

class SOWATCH_EXPORT WatchPluginInterface
{
public:
	virtual ~WatchPluginInterface();

	virtual QStringList drivers() = 0;
	virtual Watch* getWatch(const QString& driver, QSettings& settings, QObject *parent = 0) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::WatchPluginInterface, "com.javispedro.sowatch.WatchPluginInterface")

#endif // WATCHPLUGININTERFACE_H
