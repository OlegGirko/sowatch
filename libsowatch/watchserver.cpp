#include <QtCore/QDebug>

#include "notificationprovider.h"
#include "watch.h"
#include "watchlet.h"
#include "watchserver.h"


using namespace sowatch;

WatchServer::WatchServer(Watch* watch, QObject* parent) :
	QObject(parent), _watch(watch),
	_nextWatchletButton(-1),
	_oldNotificationThreshold(300),
	_currentWatchlet(0), _currentWatchletIndex(-1)
{
	connect(_watch, SIGNAL(connected()), SLOT(watchConnected()));
	connect(_watch, SIGNAL(disconnected()), SLOT(watchDisconnected()));
	connect(_watch, SIGNAL(idling()), SLOT(watchIdling()));
	connect(_watch, SIGNAL(buttonPressed(int)), SLOT(watchButtonPress(int)));
}

Watch* WatchServer::watch()
{
	return _watch;
}

QString WatchServer::nextWatchletButton() const
{
	if (_nextWatchletButton >= 0) {
		return _watch->buttons().at(_nextWatchletButton);
	} else {
		return QString();
	}
}

void WatchServer::setNextWatchletButton(const QString& value)
{
	if (value.isEmpty()) {
		_nextWatchletButton = -1;
		return;
	}
	_nextWatchletButton = _watch->buttons().indexOf(value);
	if (_nextWatchletButton < 0) {
		qWarning() << "Invalid watch button" << value;
	}
}

void WatchServer::addProvider(NotificationProvider *provider)
{
	provider->setParent(this);
	connect(provider, SIGNAL(incomingNotification(Notification*)), SLOT(postNotification(Notification*)));
	// And that's it, really.
}

void WatchServer::runWatchlet(const QString& id)
{
	if (_currentWatchlet) {
		_currentWatchlet->deactivate();
	}
	qDebug() << "activating watchlet" << id;
	_currentWatchlet = _watchlets[id];
	if (_watch->isConnected()) {
		reactivateCurrentWatchlet();
	}
}

void WatchServer::closeWatchlet()
{
	if (_currentWatchlet) {
		qDebug() << "deactivating watchlet" << _currentWatchlet->id();
		_currentWatchlet->deactivate();
		_currentWatchlet = 0;
		if (_watch->isConnected() && _pendingNotifications.empty()) {
			goToIdle();
		}
	}
}

void WatchServer::registerWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->_server == this);
	QString id = watchlet->id();
	_watchlets[id] = watchlet;
	_watchletIds.append(id);
}

void WatchServer::reactivateCurrentWatchlet()
{
	Q_ASSERT(_currentWatchlet != 0);
	_watch->displayApplication();
	_currentWatchlet->activate();
}

void WatchServer::nextWatchlet()
{
	qDebug() << "current watchlet index" << _currentWatchletIndex;
	_currentWatchletIndex++;
	if (_currentWatchletIndex >= _watchletIds.size() || _currentWatchletIndex < 0) {
		_currentWatchletIndex = -1;
		closeWatchlet();
	} else {
		QString watchlet = _watchletIds.at(_currentWatchletIndex);
		runWatchlet(watchlet);
	}
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
		goToIdle();
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

void WatchServer::goToIdle()
{
	_watch->displayIdleScreen();
	if (_nextWatchletButton >= 0) {
		_watch->grabButton(_nextWatchletButton);
	}
}

void WatchServer::watchConnected()
{
	if (!_pendingNotifications.isEmpty()) {
		nextNotification();
	} else if (_currentWatchlet) {
		reactivateCurrentWatchlet();
	} else {
		goToIdle();
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
	qDebug() << "watch idling";
	if (!_pendingNotifications.empty()) {
		_pendingNotifications.dequeue();
		nextNotification();
	}
}

void WatchServer::watchButtonPress(int button)
{
	if (button == _nextWatchletButton) {
		qDebug() << "next watchlet button pressed";
		if (_pendingNotifications.empty()) {
			// No notifications: either app or idle mode.
			nextWatchlet();
		} else {
			// Skip to next notification if any
			_pendingNotifications.dequeue();
			nextNotification();
		}
	}
}

void WatchServer::postNotification(Notification *notification)
{
	const Notification::Type type = notification->type();
	_notifications[type].append(notification);

	connect(notification, SIGNAL(changed()), SLOT(notificationChanged()));
	connect(notification, SIGNAL(cleared()), SLOT(notificationCleared()));

	qDebug() << "notification received" << notification->title() << "(" << notification->count() << ")";

	_watch->updateNotificationCount(type, getNotificationCount(type));

	QDateTime oldThreshold = QDateTime::currentDateTime().addSecs(-_oldNotificationThreshold);
	if (notification->dateTime() < oldThreshold) {
		return; // Do not care about notifications that old...
	}

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

		qDebug() << "notification changed" << n->title() << "(" << n->count() << ")";

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

		qDebug() << "notification deleted" << n->title() << "(" << n->count() << ")";

		_watch->updateNotificationCount(type, getNotificationCount(type));

		if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
			qDebug() << "removing top notification";
			_pendingNotifications.removeAll(n);
			nextNotification();
		} else {
			_pendingNotifications.removeAll(n);
		}
	}
}
