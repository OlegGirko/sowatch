#include "meegohandsetplugin.h"
#include "meegohandsetnotificationprovider.h"

using namespace sowatch;

MeegoHandsetPlugin::MeegoHandsetPlugin()
{
}

MeegoHandsetPlugin::~MeegoHandsetPlugin()
{
}

QStringList MeegoHandsetPlugin::providers()
{
	QStringList providers;
	providers << "meegohandset";
	return providers;
}

NotificationPluginInterface::NotificationProviderInfo MeegoHandsetPlugin::describeProvider(const QString &driver)
{
	NotificationProviderInfo info;
	if (driver != "meegohandset") return info;
	info.name = "MeeGo Notifications";
	return info;
}

NotificationProvider* MeegoHandsetPlugin::getProvider(const QString& driver, ConfigKey* settings, QObject *parent)
{
	Q_UNUSED(settings);
	if (driver != "meegohandset") return 0;
	return new MeegoHandsetNotificationProvider(parent);
}

Q_EXPORT_PLUGIN2(meegohandsetnotification, MeegoHandsetPlugin)
