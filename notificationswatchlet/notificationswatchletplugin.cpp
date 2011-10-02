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

Watchlet* NotificationsWatchletPlugin::getWatchlet(const QString& driver, QSettings& settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new NotificationsWatchlet(server);
}

Q_EXPORT_PLUGIN2(notificationswatchlet, NotificationsWatchletPlugin)
