#ifndef SOWATCH_NOTIFICATIONPLUGININTERFACE_H
#define SOWATCH_NOTIFICATIONPLUGININTERFACE_H

#include <QtPlugin>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include "sowatch_global.h"

namespace sowatch
{

class ConfigKey;
class Notification;
class NotificationProvider;

class SOWATCH_EXPORT NotificationPluginInterface
{
public:
	virtual ~NotificationPluginInterface();

	virtual QStringList providers() = 0;
	virtual NotificationProvider* getProvider(const QString& driver, ConfigKey *settings, QObject *parent = 0) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::NotificationPluginInterface, "com.javispedro.sowatch.NotificationPluginInterface")

#endif // SOWATCH_NOTIFICATIONPLUGININTERFACE_H
