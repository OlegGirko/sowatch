#ifndef WATCHNOTIFICATIONSINK_H
#define WATCHNOTIFICATIONSINK_H

#include <QtCore/QMap>
#include <sowatch.h>
#include "meegohandsetnotification.h"
#include <notificationsystem/notificationsink.h>

namespace sowatch {
	class MeegoHandsetNotificationProvider;
}

class WatchNotificationSink : public NotificationSink
{
    Q_OBJECT
public:
	explicit WatchNotificationSink(sowatch::MeegoHandsetNotificationProvider* parent);

	void addNotification(const Notification &notification);
	void removeNotification(uint notificationId);

	void addGroup(uint groupId, const NotificationParameters &parameters);
	void removeGroup(uint groupId);

signals:
	void incomingNotification(sowatch::Notification* notification);

protected:
	QMap<uint, MeegoHandsetNotification*> _trackedNotifications;
};

#endif // WATCHNOTIFICATIONSINK_H
