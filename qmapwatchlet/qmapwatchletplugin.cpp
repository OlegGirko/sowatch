#include "qmapwatchlet.h"
#include "mapview.h"
#include "compassview.h"
#include "qmapwatchletplugin.h"
#include "compasswatchlet.h"

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
	qmlRegisterType<CompassView>("com.javispedro.sowatch.qmap", 1, 0, "CompassView");
}

QMapWatchletPlugin::~QMapWatchletPlugin()
{
	delete _provider;
	_provider = 0;
}

QStringList QMapWatchletPlugin::watchlets()
{
	QStringList l;
	l << QMapWatchlet::myId << CompassWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo QMapWatchletPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id == QMapWatchlet::myId) {
		info.name = tr("Map");
		info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/map-icon.png");
		info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/map-" + watchModel + "-icon.png");
		info.visible = true;
	} else if (id == CompassWatchlet::myId) {
		info.name = tr("Compass");
		info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/compass-icon.png");
		info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/compass-" + watchModel + "-icon.png");
		info.visible = true;
	}
	return info;
}

Watchlet* QMapWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, Watch *watch)
{
	Q_UNUSED(config);
	if (id == QMapWatchlet::myId) {
		return new QMapWatchlet(watch);
	} else if (id == CompassWatchlet::myId) {
		return new CompassWatchlet(watch);
	}
	return 0;
}

QGeoServiceProvider *QMapWatchletPlugin::geoServiceProvider()
{
	return _provider;
}

Q_EXPORT_PLUGIN2(qmapwatchlet, QMapWatchletPlugin)
