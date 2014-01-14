#ifndef METAWATCHWATCHLETSPLUGIN_H
#define METAWATCHWATCHLETSPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchWatchletsPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)
    Q_PLUGIN_METADATA(IID "com.javispedro.sowatch.WatchletPluginInterface" FILE "metawatchwatchlets.json")

public:
	explicit MetaWatchWatchletsPlugin(QObject *parent = 0);
	~MetaWatchWatchletsPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString& watchModel);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // METAWATCHWATCHLETSPLUGIN_H
