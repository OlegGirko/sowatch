#include "liveviewnotificationwatchlet.h"
#include "liveviewwatchletsplugin.h"

using namespace sowatch;

LiveViewWatchletsPlugin::LiveViewWatchletsPlugin(QObject *parent) :
    QObject(parent)
{
}

LiveViewWatchletsPlugin::~LiveViewWatchletsPlugin()
{
}

QStringList LiveViewWatchletsPlugin::watchlets()
{
	QStringList l;
	l << LiveViewNotificationWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo LiveViewWatchletsPlugin::describeWatchlet(const QString &id, const QString& watchModel)
{
	WatchletInfo info;
	if (id == LiveViewNotificationWatchlet::myId) {
		if (watchModel == "metawatch-digital") {
			info.name = "MetaWatch Notification Watchlet";
			// Keep non visible
		}
	}
	return info;
}

Watchlet* LiveViewWatchletsPlugin::getWatchlet(const QString& id, ConfigKey *settings, Watch *watch)
{
	Q_UNUSED(settings);
	if (id == LiveViewNotificationWatchlet::myId) {
		return new LiveViewNotificationWatchlet(watch);
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(liveviewwatchlets, LiveViewWatchletsPlugin)
