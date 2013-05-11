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
    _idleWatchlet(0), _notificationWatchlet(0),
    _notifications(new NotificationsModel(this)),
    _activeWatchlet(0), _currentWatchlet(0), _currentWatchletIndex(-1),
    _syncTimeTimer(new QTimer(this))
{
	connect(_watch, SIGNAL(connected()), SLOT(handleWatchConnected()));
	connect(_watch, SIGNAL(disconnected()), SLOT(handleWatchDisconnected()));
	connect(_watch, SIGNAL(idling()), SLOT(handleWatchIdling()));
	connect(_watch, SIGNAL(buttonPressed(int)), SLOT(handleWatchButtonPress(int)));
	connect(_syncTimeTimer, SIGNAL(timeout()), SLOT(syncTime()));

	_syncTimeTimer->setSingleShot(true);
	_syncTimeTimer->setInterval(24 * 3600 * 1000); // Once a day
}

Watch* WatchServer::watch()
{
	return _watch;
}

const Watch* WatchServer::watch() const
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

Watchlet * WatchServer::idleWatchlet()
{
	return _idleWatchlet;
}

void WatchServer::setIdleWatchlet(Watchlet *watchlet)
{
	if (_idleWatchlet) {
		removeWatchlet(_idleWatchlet);
		unsetWatchletProperties(_idleWatchlet);
	}
	_idleWatchlet = watchlet;
	if (watchlet) {
		_watchletIds[watchlet->id()] = watchlet;
		setWatchletProperties(_idleWatchlet);
		// TODO Possibly activate this watchlet now if we are on the idle screen.
	}
}

Watchlet * WatchServer::notificationWatchlet()
{
	return _notificationWatchlet;
}

void WatchServer::setNotificationWatchlet(Watchlet *watchlet)
{
	if (_notificationWatchlet) {
		removeWatchlet(_notificationWatchlet);
		unsetWatchletProperties(_notificationWatchlet);
	}
	_notificationWatchlet = watchlet;
	if (watchlet) {
		_watchletIds[watchlet->id()] = watchlet;
		setWatchletProperties(_notificationWatchlet);
	}
}

void WatchServer::addWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet);
	insertWatchlet(_watchlets.size(), watchlet);
}

void WatchServer::insertWatchlet(int position, Watchlet *watchlet)
{
	Q_ASSERT(watchlet);
	Q_ASSERT(watchlet->watch() == _watch);

	const QString id = watchlet->id();
	Q_ASSERT(!_watchletIds.contains(id));

	setWatchletProperties(watchlet);

	_watchlets.insert(position, watchlet);
	_watchletIds[id] = watchlet;
}

void WatchServer::moveWatchlet(const Watchlet *watchlet, int to)
{
	const QString id = watchlet->id();
	int index = _watchlets.indexOf(const_cast<Watchlet*>(watchlet));

	Q_ASSERT(watchlet->watch() == _watch);
	Q_ASSERT(_watchletIds.contains(id));
	Q_ASSERT(index >= 0);

	_watchlets.move(index, to);
}

void WatchServer::removeWatchlet(const Watchlet *watchlet)
{
	const QString id = watchlet->id();

	Q_ASSERT(watchlet->watch() == _watch);
	Q_ASSERT(_watchletIds.contains(id));

	if (_currentWatchlet == watchlet) {
		closeWatchlet();
	}

	unsetWatchletProperties(const_cast<Watchlet*>(watchlet));

	_watchlets.removeAll(const_cast<Watchlet*>(watchlet));
	_watchletIds.remove(id);
}

void WatchServer::addProvider(NotificationProvider *provider)
{
	connect(provider, SIGNAL(incomingNotification(Notification*)),
	        this, SLOT(postNotification(Notification*)));
}

void WatchServer::removeProvider(const NotificationProvider *provider)
{
	disconnect(provider, SIGNAL(incomingNotification(Notification*)),
	           this, SLOT(postNotification(Notification*)));
}

const NotificationsModel * WatchServer::notifications() const
{
	return _notifications;
}

void WatchServer::postNotification(Notification *notification)
{
	const Notification::Priority priority = notification->priority();

	// Add notification to model
	_notifications->add(notification);
	_notificationCounts[notification] = notification->count();

	connect(notification, SIGNAL(changed()), SLOT(handleNotificationChanged()));
	connect(notification, SIGNAL(dismissed()), SLOT(handleNotificationDismissed()));
	connect(notification, SIGNAL(destroyed()), SLOT(handleNotificationDestroyed()));

	qDebug() << "notification received" << notification->title() << "(" << notification->count() << ")";

	if (priority == Notification::Silent) {
		return; // Do not display notification in the usual way
	}

	QDateTime oldThreshold = QDateTime::currentDateTime().addSecs(-_oldNotificationThreshold);
	if (notification->dateTime() < oldThreshold) {
		return; // Do not care about notifications that old...
	}

	if (_pendingNotifications.isEmpty()) {
		_pendingNotifications.enqueue(notification);
		nextNotification();
	} else if (priority == Notification::Urgent) {
		// Notification has priority, so we switch to it even if there is
		// an active notification.
		_pendingNotifications.prepend(notification);
		nextNotification();
	} else {
		_pendingNotifications.enqueue(notification);
	}
}

void WatchServer::nextNotification()
{
	if (!_watch->isConnected()) return;
	if (!_pendingNotifications.empty()) {
		Notification *n = _pendingNotifications.head();
		if (_activeWatchlet) {
			deactivateActiveWatchlet();
		}
		_watch->displayNotification(n);
		if (_notificationWatchlet) {
			activateWatchlet(_notificationWatchlet);
			_notificationWatchlet->openNotification(n);
		}
	} else if (_currentWatchlet) {
		activateCurrentWatchlet();
	} else {
		goToIdle();
	}
}

void WatchServer::runWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->watch() == _watch);
	if (_activeWatchlet) {
		deactivateActiveWatchlet();
	}
	if (_watch->isConnected()) {
		activateWatchlet(watchlet);
	}
}

void WatchServer::openWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->watch() == _watch);
	if (_activeWatchlet) {
		deactivateActiveWatchlet();
	}
	_currentWatchlet = watchlet;
	if (_watch->isConnected()) {
		activateCurrentWatchlet();
	}
}

void WatchServer::openWatchlet(const QString& id)
{
	Q_ASSERT(_watchletIds.contains(id));
	openWatchlet(_watchletIds[id]);
}

void WatchServer::closeWatchlet()
{
	if (_currentWatchlet) {
		if (_currentWatchlet == _activeWatchlet) {
			deactivateActiveWatchlet();
		}
		_currentWatchlet = 0;
		if (_watch->isConnected() && _pendingNotifications.empty()) {
			goToIdle();
		}
	}
}

void WatchServer::setWatchletProperties(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->watch() == _watch);
	watchlet->setNotificationsModel(_notifications);
}

void WatchServer::unsetWatchletProperties(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->watch() == _watch);
	watchlet->setNotificationsModel(0);
}

void WatchServer::activateWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(!_activeWatchlet);

	qDebug() << "activating watchlet" << watchlet->id();
	_activeWatchlet = watchlet;
	_activeWatchlet->activate();
}

void WatchServer::deactivateActiveWatchlet()
{
	Q_ASSERT(_activeWatchlet);

	qDebug() << "deactivating watchlet" << _activeWatchlet->id();
	_activeWatchlet->deactivate();
	_activeWatchlet = 0;
}

void WatchServer::activateCurrentWatchlet()
{
	Q_ASSERT(_currentWatchlet);
	Q_ASSERT(!_activeWatchlet);

	_watch->displayApplication();
	activateWatchlet(_currentWatchlet);
}

void WatchServer::nextWatchlet()
{
	qDebug() << "current watchlet index" << _currentWatchletIndex;
	_currentWatchletIndex++;
	if (_currentWatchletIndex >= _watchlets.size() || _currentWatchletIndex < 0) {
		_currentWatchletIndex = -1;
		closeWatchlet();
	} else {
		Watchlet* watchlet = _watchlets.at(_currentWatchletIndex);
		openWatchlet(watchlet);
	}
}

void WatchServer::syncTime()
{
	if (_watch->isConnected()) {
		qDebug() << "syncing watch time";
		_watch->setDateTime(QDateTime::currentDateTime());
		_syncTimeTimer->start();
	}
}

void WatchServer::removeNotification(Notification::Type type, Notification *n)
{
	// Warning: This function might be called with n being deleted.
	_notifications->remove(type, n);
	_notificationCounts.remove(n);

	if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
		qDebug() << "removing top notification";
		_pendingNotifications.removeAll(n);
		nextNotification();
	} else {
		_pendingNotifications.removeAll(n);
	}

	// No longer interested in this notification
	disconnect(n, 0, this, 0);
}

void WatchServer::goToIdle()
{
	Q_ASSERT(!_currentWatchlet);
	if (_activeWatchlet) {
		deactivateActiveWatchlet();
	}
	_watch->displayIdleScreen();
	if (_idleWatchlet) {
		activateWatchlet(_idleWatchlet);
	}
}

void WatchServer::handleWatchConnected()
{
	syncTime();
	if (!_pendingNotifications.isEmpty()) {
		nextNotification();
	} else if (_currentWatchlet) {
		activateCurrentWatchlet();
	} else {
		goToIdle();
	}
	emit watchConnected();
}

void WatchServer::handleWatchDisconnected()
{
	_syncTimeTimer->stop();
	if (_activeWatchlet) {
		deactivateActiveWatchlet();
	}
	_pendingNotifications.clear();
	emit watchDisconnected();
}

void WatchServer::handleWatchIdling()
{
	qDebug() << "watch idling";
	if (!_pendingNotifications.empty()) {
		_pendingNotifications.dequeue();
		nextNotification();
	}
}

void WatchServer::handleWatchButtonPress(int button)
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

void WatchServer::handleNotificationChanged()
{
	QObject *obj = sender();
	if (obj) {
		Notification* n = static_cast<Notification*>(obj);
		const uint lastCount = _notificationCounts[n];
		_notificationCounts[n] = n->count();

		qDebug() << "notification changed" << n->title() << "(" << n->count() << ")";

		if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
			// This is the notification that is being currently signaled on the watch
			// Therefore, show it again no matter what.
			nextNotification();
		} else if (n->count() > lastCount) {
			// This notification now contains an additional "item"; redisplay it.
			if (_pendingNotifications.isEmpty()) {
				_pendingNotifications.enqueue(n);
				nextNotification();
			} else {
				_pendingNotifications.enqueue(n);
			}
		}
	}
}

void WatchServer::handleNotificationDismissed()
{
	QObject *obj = sender();
	if (obj) {
		Notification* n = static_cast<Notification*>(obj);
		const Notification::Type type = n->type();
		qDebug() << "notification dismissed" << n->title() << "(" << n->count() << ")";
		removeNotification(type, n);
	}
}

void WatchServer::handleNotificationDestroyed()
{
	QObject *obj = sender();
	if (obj) {
		Notification* n = static_cast<Notification*>(obj);
		// Cannot call any methods of n; it is a dangling pointer now.
		if (_notificationCounts.contains(n)) {
			qWarning() << "Notification destroyed without being dismissed!";
			Notification::Type type = _notifications->getTypeOfDeletedNotification(n);
			removeNotification(type, n);
		}
	}
}
