#include "ckitcallprovider.h"
#include "ckitcallplugin.h"

using namespace sowatch;

CKitCallPlugin::CKitCallPlugin(QObject *parent) :
    QObject(parent)
{
}

CKitCallPlugin::~CKitCallPlugin()
{
}

QStringList CKitCallPlugin::providers()
{
	QStringList providers;
	providers << "ckitcall";
	return providers;
}

NotificationPluginInterface::NotificationProviderInfo CKitCallPlugin::describeProvider(const QString &driver)
{
	Q_UNUSED(driver);
	NotificationProviderInfo info;
	info.name = "MeeGo Incoming calls";
	return info;
}

NotificationProvider* CKitCallPlugin::getProvider(const QString& driver, ConfigKey *settings, QObject *parent)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new CKitCallProvider(parent);
}

Q_EXPORT_PLUGIN2(ckitcallnotification, CKitCallPlugin)
