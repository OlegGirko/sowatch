#include "metawatchfacewatchlet.h"
#include "metawatchnotificationwatchlet.h"
#include "metawatchwatchletsplugin.h"

using namespace sowatch;

MetaWatchWatchletsPlugin::MetaWatchWatchletsPlugin(QObject *parent) :
    QObject(parent)
{
}

MetaWatchWatchletsPlugin::~MetaWatchWatchletsPlugin()
{
}

QStringList MetaWatchWatchletsPlugin::watchlets()
{
	QStringList l;
	l << MetaWatchFaceWatchlet::myId << MetaWatchNotificationWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo MetaWatchWatchletsPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id == MetaWatchFaceWatchlet::myId) {
		info.name = "MetaWatch Face Watchlet";
		info.hidden = true;
	} else if (id == MetaWatchNotificationWatchlet::myId) {
		info.name = "MetaWatch Notification Watchlet";
		info.hidden = true;
	}
	return info;
}

Watchlet* MetaWatchWatchletsPlugin::getWatchlet(const QString& id, ConfigKey *settings, Watch *watch)
{
	Q_UNUSED(settings);
	if (id == MetaWatchFaceWatchlet::myId) {
		return new MetaWatchFaceWatchlet(watch);
	} else if (id == MetaWatchNotificationWatchlet::myId) {
		return new MetaWatchNotificationWatchlet(watch);
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(notificationswatchlet, MetaWatchWatchletsPlugin)
