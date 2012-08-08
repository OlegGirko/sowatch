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

NotificationProvider* CKitCallPlugin::getProvider(const QString& driver, ConfigKey *settings, QObject *parent)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new CKitCallProvider(parent);
}

Q_EXPORT_PLUGIN2(ckitcallnotification, CKitCallPlugin)
