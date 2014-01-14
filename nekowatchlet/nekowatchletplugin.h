#ifndef NEKOWATCHLETPLUGIN_H
#define NEKOWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class NekoWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)
    Q_PLUGIN_METADATA(IID "com.javispedro.sowatch.WatchletPluginInterface" FILE "nekowatchlet.json")

public:
	explicit NekoWatchletPlugin(QObject *parent = 0);
	~NekoWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString &watchModel);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // NEKOWATCHLETPLUGIN_H
