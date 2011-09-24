#ifndef SOWATCH_WATCHLETPLUGININTERFACE_H
#define SOWATCH_WATCHLETPLUGININTERFACE_H

#include <QtPlugin>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include "sowatch_global.h"

namespace sowatch
{

class Watchlet;
class WatchServer;

class SOWATCH_EXPORT WatchletPluginInterface
{
public:
	virtual ~WatchletPluginInterface();

	virtual QStringList watchlets() = 0;
	virtual Watchlet* getProvider(const QString& id, QSettings& settings, WatchServer *server) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::WatchletPluginInterface, "com.javispedro.sowatch.WatchletPluginInterface")

#endif // SOWATCH_WATCHLETPLUGININTERFACE_H
