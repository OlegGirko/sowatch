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

Watchlet* SysInfoPlugin::getWatchlet(const QString& driver, ConfigKey *settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new SysInfoWatchlet(server);
}

Q_EXPORT_PLUGIN2(sysinfowatchlet, SysInfoPlugin)
