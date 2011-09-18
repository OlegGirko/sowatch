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

NotificationProvider* MeegoHandsetPlugin::getProvider(const QString& driver, QSettings& settings, QObject *parent)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new MeegoHandsetNotificationProvider(parent);
}

Q_EXPORT_PLUGIN2(meegohandsetnotification, MeegoHandsetPlugin)
