#ifndef NOTIFICATIONSWATCHLETPLUGIN_H
#define NOTIFICATIONSWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class NotificationsWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit NotificationsWatchletPlugin(QObject *parent = 0);
	~NotificationsWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // NOTIFICATIONSWATCHLETPLUGIN_H
