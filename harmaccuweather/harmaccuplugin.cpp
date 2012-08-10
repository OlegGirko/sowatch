#include "harmaccuprovider.h"
#include "harmaccuplugin.h"

using namespace sowatch;

HarmAccuPlugin::HarmAccuPlugin(QObject *parent) :
    QObject(parent)
{
}

HarmAccuPlugin::~HarmAccuPlugin()
{
}

QStringList HarmAccuPlugin::providers()
{
	QStringList providers;
	providers << "harmaccu";
	return providers;
}

NotificationPluginInterface::NotificationProviderInfo HarmAccuPlugin::describeProvider(const QString &driver)
{
	NotificationProviderInfo info;
	if (driver != "harmaccu") return info;
	info.name = "Accuweather";
	return info;
}

NotificationProvider* HarmAccuPlugin::getProvider(const QString& id, ConfigKey *settings, QObject *parent)
{
	Q_UNUSED(settings);
	if (id != "harmaccu") return 0;
	return new HarmAccuProvider(parent);
}

Q_EXPORT_PLUGIN2(harmaccuweather, HarmAccuPlugin)
