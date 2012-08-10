#ifndef QMAPWATCHLETPLUGIN_H
#define QMAPWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class QMapWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit QMapWatchletPlugin(QObject *parent = 0);
	~QMapWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, WatchServer *server);
};

}

#endif // QMAPWATCHLETPLUGIN_H
