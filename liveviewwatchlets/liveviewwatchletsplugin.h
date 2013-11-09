#ifndef LIVEVIEWWATCHLETSPLUGIN_H
#define LIVEVIEWWATCHLETSPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class LiveViewWatchletsPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit LiveViewWatchletsPlugin(QObject *parent = 0);
	~LiveViewWatchletsPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString& watchModel);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // LIVEVIEWWATCHLETSPLUGIN_H
