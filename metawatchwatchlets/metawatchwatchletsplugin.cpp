#include "metawatchfacewatchlet.h"
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
	l << MetaWatchFaceWatchlet::myId;
	return l;
}

WatchletPluginInterface::WatchletInfo MetaWatchWatchletsPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id == MetaWatchFaceWatchlet::myId) {
		info.name = "MetaWatch Face Watchlet";
		info.hidden = true;
	}
	return info;
}

Watchlet* MetaWatchWatchletsPlugin::getWatchlet(const QString& id, ConfigKey *settings, Watch *watch)
{
	Q_UNUSED(settings);
	if (id == MetaWatchFaceWatchlet::myId) {
		return new MetaWatchFaceWatchlet(watch);
	}
}

Q_EXPORT_PLUGIN2(notificationswatchlet, MetaWatchWatchletsPlugin)
