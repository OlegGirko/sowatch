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

NotificationProvider* HarmAccuPlugin::getProvider(const QString& id, QSettings& settings, QObject *parent)
{
	if (id != "harmaccu") return 0;
	int updateTime = settings.value("updateTime").toInt();
	if (updateTime <= 0) updateTime = 2 * 3600; // Two hours by default
	return new HarmAccuProvider(updateTime, parent);
}

Q_EXPORT_PLUGIN2(harmaccuweather, HarmAccuPlugin)
