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
	Watchlet* getWatchlet(const QString& driver, QSettings& settings, WatchServer* server);
};

}

#endif // NOTIFICATIONSWATCHLETPLUGIN_H
