#ifndef TESTNOTIFICATIONPLUGIN_H
#define TESTNOTIFICATIONPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class TestNotificationPlugin : public QObject, public NotificationPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::NotificationPluginInterface)

public:
	TestNotificationPlugin(QObject *parent = 0);
	~TestNotificationPlugin();

	QStringList providers();
	NotificationProviderInfo describeProvider(const QString &driver);
	NotificationProvider* getProvider(const QString& driver, ConfigKey *settings, QObject *parent = 0);
};

}

#endif // TESTNOTIFICATIONPLUGIN_H
