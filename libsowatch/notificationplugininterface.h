#ifndef NOTIFICATIONPLUGININTERFACE_H
#define NOTIFICATIONPLUGININTERFACE_H

#include <QtPlugin>
#include "sowatch_global.h"
#include "notification.h"

namespace sowatch
{

class Notification;

class SOWATCH_EXPORT NotificationPluginInterface
{
public:
	virtual ~NotificationPluginInterface();

	int getCount(Notification::Type type);

signals:
	void incomingNotification(const Notification& n);
	void unreadCountChanged(Notification::Type type);
};

}

Q_DECLARE_INTERFACE(sowatch::NotificationPluginInterface, "com.javispedro.sowatch.NotificationPluginInterface")

#endif // NOTIFICATIONPLUGININTERFACE_H
