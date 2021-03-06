#ifndef NOTIFICATIONSWATCHLETPLUGIN_H
#define NOTIFICATIONSWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class NotificationsWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)
    Q_PLUGIN_METADATA(IID "com.javispedro.sowatch.WatchletPluginInterface" FILE "notificationswatchlet.json")

public:
	explicit NotificationsWatchletPlugin(QObject *parent = 0);
	~NotificationsWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString &watchModel);
	Watchlet* getWatchlet(const QString& id, ConfigKey *settings, Watch* watch);
};

}

#endif // NOTIFICATIONSWATCHLETPLUGIN_H
