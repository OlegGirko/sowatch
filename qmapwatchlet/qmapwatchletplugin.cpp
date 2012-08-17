#include "qmapwatchlet.h"
#include "mapview.h"
#include "qmapwatchletplugin.h"

using namespace sowatch;
QTM_USE_NAMESPACE

QGeoServiceProvider*  QMapWatchletPlugin::_provider;

QMapWatchletPlugin::QMapWatchletPlugin(QObject *parent) :
    QObject(parent)
{
	QStringList avail =  QGeoServiceProvider::availableServiceProviders();
	if (!avail.isEmpty()) {
		_provider = new QGeoServiceProvider(avail.first());
	} else {
		_provider = 0;
	}
	qmlRegisterType<MapView>("com.javispedro.sowatch.qmap", 1, 0, "MapView");
}

QMapWatchletPlugin::~QMapWatchletPlugin()
{
	delete _provider;
	_provider = 0;
}

QStringList QMapWatchletPlugin::watchlets()
{
	QStringList l;
	l << QMapWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo QMapWatchletPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != QMapWatchlet::myId) return info;
	info.name = "Moving map";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/icon.png");
	return info;
}

Watchlet* QMapWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, WatchServer *server)
{
	Q_UNUSED(config);
	if (id != QMapWatchlet::myId) return 0;
	return new QMapWatchlet(server);
}

QGeoServiceProvider *QMapWatchletPlugin::geoServiceProvider()
{
	return _provider;
}

Q_EXPORT_PLUGIN2(qmapwatchlet, QMapWatchletPlugin)
