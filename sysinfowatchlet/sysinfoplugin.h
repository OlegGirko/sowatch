#ifndef CKITCALLPLUGIN_H
#define CKITCALLPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class SysInfoPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	SysInfoPlugin(QObject *parent = 0);
	~SysInfoPlugin();

	QStringList watchlets();
	Watchlet* getWatchlet(const QString& driver, ConfigKey *settings, WatchServer* server);
};

}

#endif // CKITCALLPLUGIN_H
