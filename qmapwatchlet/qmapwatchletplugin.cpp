#include "qmapwatchlet.h"
#include "qmapwatchletplugin.h"

using namespace sowatch;

QMapWatchletPlugin::QMapWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

QMapWatchletPlugin::~QMapWatchletPlugin()
{
}

QStringList QMapWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.qmap";
	return l;
}

Watchlet* QMapWatchletPlugin::getWatchlet(const QString& driver, QSettings& settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new QMapWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmapwatchlet, QMapWatchletPlugin)
