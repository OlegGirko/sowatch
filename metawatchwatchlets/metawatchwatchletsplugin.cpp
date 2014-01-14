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

WatchletPluginInterface::WatchletInfo MetaWatchWatchletsPlugin::describeWatchlet(const QString &id, const QString& watchModel)
{
	WatchletInfo info;
	if (id == MetaWatchFaceWatchlet::myId) {
		if (watchModel == "metawatch-digital") {
			info.name = "MetaWatch Face Watchlet";
			// Keep non visible
		}
	} else if (id == MetaWatchNotificationWatchlet::myId) {
		if (watchModel == "metawatch-digital") {
			info.name = "MetaWatch Notification Watchlet";
			// Keep non visible
		}
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
