#include "nekowatchlet.h"
#include "nekowatchletplugin.h"

using namespace sowatch;

NekoWatchletPlugin::NekoWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

NekoWatchletPlugin::~NekoWatchletPlugin()
{
}

QStringList NekoWatchletPlugin::watchlets()
{
	QStringList l;
	l << NekoWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo NekoWatchletPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != NekoWatchlet::myId) return info;
	info.name = "Neko";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/nekowatchlet/icon.png");
	return info;
}

Watchlet* NekoWatchletPlugin::getWatchlet(const QString& driver, ConfigKey *settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new NekoWatchlet(server);
}

Q_EXPORT_PLUGIN2(notificationswatchlet, NekoWatchletPlugin)
