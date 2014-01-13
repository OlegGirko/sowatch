#include "notificationswatchlet.h"
#include "notificationswatchletplugin.h"

using namespace sowatch;

NotificationsWatchletPlugin::NotificationsWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

NotificationsWatchletPlugin::~NotificationsWatchletPlugin()
{
}

QStringList NotificationsWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.notifications";
	return l;
}

WatchletPluginInterface::WatchletInfo NotificationsWatchletPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.notifications") return info;
	info.name = "Pending notifications";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/notificationswatchlet/" + watchModel + "-icon.png");
	info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/notificationswatchlet/icon.png");
	info.visible = true;
	return info;
}

Watchlet* NotificationsWatchletPlugin::getWatchlet(const QString& driver, ConfigKey *settings, Watch *watch)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new NotificationsWatchlet(watch);
}

//Q_EXPORT_PLUGIN2(notificationswatchlet, NotificationsWatchletPlugin)
