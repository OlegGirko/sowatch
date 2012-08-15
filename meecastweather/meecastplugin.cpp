#include "meecastprovider.h"
#include "meecastplugin.h"

using namespace sowatch;

MeeCastPlugin::MeeCastPlugin(QObject *parent) :
    QObject(parent)
{
}

MeeCastPlugin::~MeeCastPlugin()
{
}

QStringList MeeCastPlugin::providers()
{
	QStringList providers;
	providers << MeeCastProvider::myId;
	return providers;
}

NotificationPluginInterface::NotificationProviderInfo MeeCastPlugin::describeProvider(const QString &driver)
{
	NotificationProviderInfo info;
	if (driver != MeeCastProvider::myId) return info;
	info.name = "MeeCast";
	return info;
}

NotificationProvider* MeeCastPlugin::getProvider(const QString& id, ConfigKey *settings, QObject *parent)
{
	Q_UNUSED(settings);
	if (id != MeeCastProvider::myId) return 0;
	return new MeeCastProvider(parent);
}

Q_EXPORT_PLUGIN2(meecastweather, MeeCastPlugin)
