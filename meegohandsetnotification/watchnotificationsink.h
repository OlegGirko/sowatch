#ifndef WATCHNOTIFICATIONSINK_H
#define WATCHNOTIFICATIONSINK_H

#include <QtCore/QMap>
#include <sowatch.h>
#include <notificationsystem/notification.h>
#include <notificationsystem/notificationsink.h>
#include <notificationsystem/notificationparameters.h>

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

	int getCount(sowatch::Notification::Type type);

signals:
	void incomingNotification(const sowatch::Notification& notification);
	void countsChanged(sowatch::Notification::Type type);

protected:
	static const uint maxTypes = sowatch::Notification::TypeCount;
	static sowatch::Notification::Type notificationTypeFromEventType(const QString& eventType);

	sowatch::MeegoHandsetNotificationProvider* _parent;
	QMap<uint, Notification> _trackedNotifications;
	uint _counts[maxTypes];
};

#endif // WATCHNOTIFICATIONSINK_H
