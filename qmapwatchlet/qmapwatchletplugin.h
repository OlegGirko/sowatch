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
	Watchlet* getWatchlet(const QString& driver, QSettings& settings, WatchServer* server);
};

}

#endif // QMAPWATCHLETPLUGIN_H
