#ifndef SOWATCH_WATCHPLUGININTERFACE_H
#define SOWATCH_WATCHPLUGININTERFACE_H

#include <QtPlugin>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>
#include "sowatch_global.h"

namespace sowatch
{

class Watch;
class WatchScanner;
class ConfigKey;

class SOWATCH_EXPORT WatchPluginInterface
{
public:
	virtual ~WatchPluginInterface();

	virtual QStringList drivers() = 0;
	virtual WatchScanner* getScanner(QObject *parent = 0) = 0;
	virtual Watch* getWatch(const QString& driver, ConfigKey* settings, QObject *parent = 0) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::WatchPluginInterface, "com.javispedro.sowatch.WatchPluginInterface")

#endif // SOWATCH_WATCHPLUGININTERFACE_H
