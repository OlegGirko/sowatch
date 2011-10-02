#include "meegohandsetnotificationprovider.h"
#include "watchnotificationsink.h"

WatchNotificationSink::WatchNotificationSink(sowatch::MeegoHandsetNotificationProvider *parent) :
	NotificationSink(parent)
{

}

void WatchNotificationSink::addNotification(const Notification &notification)
{
	const uint notificationId = notification.notificationId();

	if (_trackedNotifications.contains(notificationId)) {
		MeegoHandsetNotification* n = _trackedNotifications[notificationId];
		n->changeTo(notification);
	} else {
		MeegoHandsetNotification* n = new MeegoHandsetNotification(notification, this);
		_trackedNotifications[notificationId] = n;
		emit incomingNotification(static_cast<sowatch::Notification*>(n));
	}
}

void WatchNotificationSink::removeNotification(uint notificationId)
{
	if (_trackedNotifications.contains(notificationId)) {
		MeegoHandsetNotification* n = _trackedNotifications[notificationId];
		_trackedNotifications.remove(notificationId);
		n->remove();
		n->deleteLater();
	}
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
