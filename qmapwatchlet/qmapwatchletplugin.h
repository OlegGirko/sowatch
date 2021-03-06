#ifndef QMAPWATCHLETPLUGIN_H
#define QMAPWATCHLETPLUGIN_H

#include <QtLocation/QGeoServiceProvider>
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
	WatchletInfo describeWatchlet(const QString &id, const QString &watchModel);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, Watch *watch);

	static QtMobility::QGeoServiceProvider * geoServiceProvider();

private:
	static QtMobility::QGeoServiceProvider *_provider;
};

}

#endif // QMAPWATCHLETPLUGIN_H
