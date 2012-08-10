#ifndef SOWATCH_WATCHLETPLUGININTERFACE_H
#define SOWATCH_WATCHLETPLUGININTERFACE_H

#include <QtPlugin>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include "sowatch_global.h"

namespace sowatch
{

class ConfigKey;
class Watchlet;
class WatchServer;

class SOWATCH_EXPORT WatchletPluginInterface
{
public:
	virtual ~WatchletPluginInterface();

	struct WatchletInfo {
		QString name;
		QUrl icon;
	};

	virtual QStringList watchlets() = 0;
	virtual WatchletInfo describeWatchlet(const QString& id) = 0;
	virtual Watchlet* getWatchlet(const QString& id, ConfigKey *settings, WatchServer *server) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::WatchletPluginInterface, "com.javispedro.sowatch.WatchletPluginInterface")

#endif // SOWATCH_WATCHLETPLUGININTERFACE_H
