#include "testnotificationprovider.h"
#include "testnotificationplugin.h"

using namespace sowatch;

static const QLatin1String providerId("testnotification");

TestNotificationPlugin::TestNotificationPlugin(QObject *parent) :
    QObject(parent)
{
}

TestNotificationPlugin::~TestNotificationPlugin()
{
}

QStringList TestNotificationPlugin::providers()
{
	QStringList providers;
	providers << providerId;
	return providers;
}

NotificationPluginInterface::NotificationProviderInfo TestNotificationPlugin::describeProvider(const QString &driver)
{
	NotificationProviderInfo info;
	if (driver != providerId) return info;
	info.name = "Test notifications";
	return info;
}

NotificationProvider* TestNotificationPlugin::getProvider(const QString& id, ConfigKey *settings, QObject *parent)
{
	Q_UNUSED(settings);
	if (id != providerId) return 0;
	return new TestNotificationProvider(parent);
}

Q_EXPORT_PLUGIN2(testnotification, TestNotificationPlugin)
