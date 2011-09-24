#include <QtCore/QDebug>

#include "notificationprovider.h"
#include "watch.h"
#include "watchlet.h"
#include "watchserver.h"


using namespace sowatch;

WatchServer::WatchServer(Watch* watch, QObject* parent) :
	QObject(parent), _watch(watch), _currentWatchlet(0)
{
	connect(_watch, SIGNAL(connected()), SLOT(watchConnected()));
	connect(_watch, SIGNAL(disconnected()), SLOT(watchDisconnected()));
	connect(_watch, SIGNAL(idling()), SLOT(watchIdling()));
}

Watch* WatchServer::watch()
{
	return _watch;
}

void WatchServer::addProvider(NotificationProvider *provider)
{
	provider->setParent(this);
	connect(provider, SIGNAL(incomingNotification(Notification*)), SLOT(notificationReceived(Notification*)));
	// And that's it, really.
}

void WatchServer::runWatchlet(const QString& id)
{
	if (_currentWatchlet) {
		closeWatchlet();
	}
	_currentWatchlet = _watchlets[id];
	if (_watch->isConnected()) {
		reactivateCurrentWatchlet();
	}
}

void WatchServer::closeWatchlet()
{
	Q_ASSERT(_currentWatchlet != 0);
	if (_watch->isConnected()) {
		_currentWatchlet->deactivate();
	}
	_currentWatchlet = 0;
}

void WatchServer::registerWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->_server == this);
	_watchlets[watchlet->id()] = watchlet;
}

void WatchServer::reactivateCurrentWatchlet()
{
	Q_ASSERT(_currentWatchlet != 0);
	_watch->displayApplication();
	_currentWatchlet->activate();
}

void WatchServer::nextNotification()
{
	if (!_watch->isConnected()) return;
	if (!_pendingNotifications.empty()) {
		Notification *n = _pendingNotifications.head();
		_watch->displayNotification(n);
	} else if (_currentWatchlet) {
		reactivateCurrentWatchlet();
	} else {
		_watch->displayIdleScreen();
	}
}

uint WatchServer::getNotificationCount(Notification::Type type)
{
	uint count = 0;
	foreach (Notification* n, _notifications[type]) {
		count += n->count();
	}
	return count;
}

void WatchServer::watchConnected()
{
	if (!_pendingNotifications.isEmpty()) {
		nextNotification();
	} else if (_currentWatchlet) {
		reactivateCurrentWatchlet();
	}
}

void WatchServer::watchDisconnected()
{
	if (_currentWatchlet) {
		_currentWatchlet->deactivate();
	}
	_pendingNotifications.clear();
}

void WatchServer::watchIdling()
{
	qDebug() << "Watch idling";
	if (!_pendingNotifications.empty()) {
		_pendingNotifications.dequeue();
		nextNotification();
	}
}

void WatchServer::notificationReceived(Notification *notification)
{
	const Notification::Type type = notification->type();
	_notifications[type].append(notification);

	connect(notification, SIGNAL(changed()), SLOT(notificationChanged()));
	connect(notification, SIGNAL(cleared()), SLOT(notificationCleared()));

	qDebug() << "notification received" << notification->title() << notification->count();

	_watch->updateNotificationCount(type, getNotificationCount(type));
	if (_pendingNotifications.isEmpty()) {
		_pendingNotifications.enqueue(notification);
		nextNotification();
	} else if (type == Notification::CallNotification) {
		// Oops, priority!!!!
		_pendingNotifications.prepend(notification);
		nextNotification();
	} else {
		_pendingNotifications.enqueue(notification);
	}
}

void WatchServer::notificationChanged()
{
	QObject *obj = sender();
	if (obj) {
		Notification* n = static_cast<Notification*>(obj);
		const Notification::Type type = n->type();

		qDebug() << "notification changed" << n->title() << n->count();

		_watch->updateNotificationCount(type, getNotificationCount(type));
		if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
			nextNotification();
		}
	}
}

void WatchServer::notificationCleared()
{
	QObject *obj = sender();
	if (obj) {
		Notification* n = static_cast<Notification*>(obj);
		const Notification::Type type = n->type();
		_notifications[type].removeOne(n);

		qDebug() << "notification deleted" << n->title() << n->count();

		_watch->updateNotificationCount(type, getNotificationCount(type));
		if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
			_pendingNotifications.removeAll(n);
			nextNotification();
		} else {
			_pendingNotifications.removeAll(n);
		}
	}
}
