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
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString &id, ConfigKey *settings, Watch *watch);
};

}

#endif // CKITCALLPLUGIN_H
