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

SysInfoPlugin::WatchletInfo SysInfoPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.sysinfo") return info;
	info.name = "Phone info";
	info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/sysinfowatchlet/icon.png");
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/sysinfowatchlet/" + watchModel + "-icon.png");
	info.visible = true;
	return info;
}

Watchlet* SysInfoPlugin::getWatchlet(const QString& id, ConfigKey *settings, Watch *watch)
{
	Q_UNUSED(id);
	Q_UNUSED(settings);
	return new SysInfoWatchlet(watch);
}

Q_EXPORT_PLUGIN2(sysinfowatchlet, SysInfoPlugin)
