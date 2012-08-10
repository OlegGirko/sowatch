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

WatchletPluginInterface::WatchletInfo QMapWatchletPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qmap") return info;
	info.name = "Moving map";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmapwatchlet/icon.png");
	return info;
}

Watchlet* QMapWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, WatchServer *server)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qmap") return 0;
	return new QMapWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmapwatchlet, QMapWatchletPlugin)
