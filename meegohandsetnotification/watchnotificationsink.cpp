#include "meegohandsetnotificationprovider.h"
#include "watchnotificationsink.h"

WatchNotificationSink::WatchNotificationSink(sowatch::MeegoHandsetNotificationProvider *parent) :
	NotificationSink(parent), _parent(parent)
{
	for (uint i = 0; i < maxTypes; i++) {
		_counts[i] = 0;
	}
}

void WatchNotificationSink::addNotification(const Notification &notification)
{
	const NotificationParameters& p = notification.parameters();
	sowatch::Notification::Type type = notificationTypeFromEventType(p.value("eventType").toString());
	const uint count = p.value("count").toUInt();
	const uint notificationId = notification.notificationId();

	if (_trackedNotifications.contains(notificationId)) {
		const NotificationParameters& oldParams = _trackedNotifications[notificationId].parameters();
		_counts[type] -= oldParams.value("count").toUInt();
	}

	_counts[type] += count;
	_trackedNotifications[notificationId] = notification;

	emit countsChanged(type);

	QDateTime dt = QDateTime::fromTime_t(p.value("timestamp").toUInt());
	QDateTime tenSecondsAgo = QDateTime::currentDateTimeUtc().addSecs(-10);
	if (dt >= tenSecondsAgo) {
		// If the notification happened recently, show it.
		sowatch::Notification n(type, dt, p.value("summary").toString(), p.value("body").toString());
		emit incomingNotification(n);
	}
}

void WatchNotificationSink::removeNotification(uint notificationId)
{
	Notification notification = _trackedNotifications[notificationId];
	const NotificationParameters& p = notification.parameters();
	sowatch::Notification::Type type = notificationTypeFromEventType(p.value("eventType").toString());
	uint count = p.value("count").toUInt();

	_counts[type] -= count;
	_trackedNotifications.remove(notificationId);

	emit countsChanged(type);
}

void WatchNotificationSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
	// We do not care about notification groups
	Q_UNUSED(groupId);
	Q_UNUSED(parameters);
}

void WatchNotificationSink::removeGroup(uint groupId)
{
	Q_UNUSED(groupId);
}

int WatchNotificationSink::getCount(sowatch::Notification::Type type)
{
	return _counts[type];
}

sowatch::Notification::Type WatchNotificationSink::notificationTypeFromEventType(const QString& eventType)
{
	qDebug() << "incoming " << eventType;
	if (eventType == "email.arrived")
		return sowatch::Notification::EmailNotification;
	else if (eventType == "x-nokia.call.missed")
		return sowatch::Notification::MissedCallNotification;
	else if (eventType == "x-nokia.messaging.im")
		return sowatch::Notification::ImNotification;
	else if (eventType == "x-nokia.messaging.sms")
		return sowatch::Notification::SmsNotification;
	else if (eventType == "x-nokia.messaging.mms")
		return sowatch::Notification::MmsNotification;
	else
		return sowatch::Notification::OtherNotification;
}
