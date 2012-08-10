#include "sysinfowatchlet.h"
#include "sysinfoplugin.h"

using namespace sowatch;

SysInfoPlugin::SysInfoPlugin(QObject *parent) :
    QObject(parent)
{
}

SysInfoPlugin::~SysInfoPlugin()
{
}

QStringList SysInfoPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.sysinfo";
	return l;
}

SysInfoPlugin::WatchletInfo SysInfoPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.sysinfo") return info;
	info.name = "Phone info";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/sysinfowatchlet/icon.png");
	return info;
}

Watchlet* SysInfoPlugin::getWatchlet(const QString& id, ConfigKey *settings, WatchServer *server)
{
	Q_UNUSED(id);
	Q_UNUSED(settings);
	return new SysInfoWatchlet(server);
}

Q_EXPORT_PLUGIN2(sysinfowatchlet, SysInfoPlugin)
