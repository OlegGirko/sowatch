#ifndef NEKOWATCHLETPLUGIN_H
#define NEKOWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class NekoWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit NekoWatchletPlugin(QObject *parent = 0);
	~NekoWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // NEKOWATCHLETPLUGIN_H
