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
    _notifications(new NotificationsModel(this)),
	_currentWatchlet(0), _currentWatchletActive(false), _currentWatchletIndex(-1),
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

void WatchServer::addWatchlet(Watchlet *watchlet)
{
	insertWatchlet(_watchlets.size(), watchlet);
}

void WatchServer::insertWatchlet(int position, Watchlet *watchlet)
{
	const QString id = watchlet->id();
	Q_ASSERT(watchlet->_server == this);
	Q_ASSERT(!_watchletIds.contains(id));

	_watchlets.insert(position, watchlet);
	_watchletIds[id] = watchlet;
}

void WatchServer::removeWatchlet(const Watchlet *watchlet)
{
	const QString id = watchlet->id();

	Q_ASSERT(watchlet->_server == this);
	Q_ASSERT(_watchletIds.contains(id));

	if (_currentWatchlet == watchlet) {
		closeWatchlet();
	}

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
	const Notification::Type type = notification->type();

	// Add notification to model
	_notifications->add(notification);
	_notificationCounts[notification] = notification->count();

	connect(notification, SIGNAL(changed()), SLOT(handleNotificationChanged()));
	connect(notification, SIGNAL(dismissed()), SLOT(handleNotificationDismissed()));
	connect(notification, SIGNAL(destroyed()), SLOT(handleNotificationDestroyed()));

	qDebug() << "notification received" << notification->title() << "(" << notification->count() << ")";

	_watch->updateNotificationCount(type, getNotificationCount(type));

	if (type == Notification::WeatherNotification) {
		// Weather notifications, we handle differently.
		WeatherNotification* weather = static_cast<WeatherNotification*>(notification);
		_weather = weather;
		_watch->updateWeather(weather);
		return; // And do not display it the usual way
	}

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

void WatchServer::nextNotification()
{
	if (!_watch->isConnected()) return;
	if (!_pendingNotifications.empty()) {
		Notification *n = _pendingNotifications.head();
		if (_currentWatchlet && _currentWatchletActive) {
			deactivateCurrentWatchlet();
		}
		_watch->displayNotification(n);
	} else if (_currentWatchlet) {
		reactivateCurrentWatchlet();
	} else {
		goToIdle();
	}
}

void WatchServer::runWatchlet(Watchlet *watchlet)
{
	Q_ASSERT(watchlet->_server == this);
	if (_currentWatchlet && _currentWatchletActive) {
		deactivateCurrentWatchlet();
	}
	_currentWatchlet = watchlet;
	if (_watch->isConnected()) {
		reactivateCurrentWatchlet();
	}
}

void WatchServer::runWatchlet(const QString& id)
{
	Q_ASSERT(_watchletIds.contains(id));
	runWatchlet(_watchletIds[id]);
}

void WatchServer::closeWatchlet()
{
	if (_currentWatchlet) {
		if (_currentWatchletActive) {
			deactivateCurrentWatchlet();
		}
		_currentWatchlet = 0;
		if (_watch->isConnected() && _pendingNotifications.empty()) {
			goToIdle();
		}
	}
}

void WatchServer::deactivateCurrentWatchlet()
{
	Q_ASSERT(_currentWatchlet != 0);
	Q_ASSERT(_currentWatchletActive);
	qDebug() << "deactivating watchlet" << _currentWatchlet->id();
	_currentWatchlet->deactivate();
	_currentWatchletActive = false;
}

void WatchServer::reactivateCurrentWatchlet()
{
	Q_ASSERT(_currentWatchlet != 0);
	Q_ASSERT(!_currentWatchletActive);
	qDebug() << "activating watchlet" << _currentWatchlet->id();
	_watch->displayApplication();
	_currentWatchlet->activate();
	_currentWatchletActive = true;
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
		runWatchlet(watchlet);
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

uint WatchServer::getNotificationCount(Notification::Type type)
{
	return _notifications->fullCountByType(type);
}

void WatchServer::removeNotification(Notification::Type type, Notification *n)
{
	// Warning: This function might be called with n being deleted.
	_notifications->remove(type, n);
	_notificationCounts.remove(n);

	_watch->updateNotificationCount(type, getNotificationCount(type));

	if (!_pendingNotifications.isEmpty() && _pendingNotifications.head() == n) {
		qDebug() << "removing top notification";
		_pendingNotifications.removeAll(n);
		nextNotification();
	} else {
		_pendingNotifications.removeAll(n);
	}
	if (type == Notification::WeatherNotification) {
		WeatherNotification* w = static_cast<WeatherNotification*>(n);
		if (_weather == w) {
			_weather = 0;
		}
	}

	// No longer interested in this notification
	disconnect(n, 0, this, 0);
}

void WatchServer::goToIdle()
{
	Q_ASSERT(!_currentWatchletActive);
	_watch->displayIdleScreen();
}

void WatchServer::handleWatchConnected()
{
	syncTime();
	if (!_pendingNotifications.isEmpty()) {
		nextNotification();
	} else if (_currentWatchlet) {
		reactivateCurrentWatchlet();
	} else {
		goToIdle();
	}
	emit watchConnected();
}

void WatchServer::handleWatchDisconnected()
{
	_syncTimeTimer->stop();
	if (_currentWatchlet && _currentWatchletActive) {
		deactivateCurrentWatchlet();
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
		const Notification::Type type = n->type();
		const uint lastCount = _notificationCounts[n];
		_notificationCounts[n] = n->count();

		qDebug() << "notification changed" << n->title() << "(" << n->count() << ")";

		_watch->updateNotificationCount(type, getNotificationCount(type));

		if (type == Notification::WeatherNotification) {
			WeatherNotification* w = static_cast<WeatherNotification*>(n);
			if (!_weather || _weather->dateTime() < w->dateTime()) {
				// Prefer showing the most recent data
				_weather = w;
			}
			if (_weather == w) {
				// This is the weather notification we are currently displaying on the watch
				// Therefore, update the displayed information
				_watch->updateWeather(w);
			}
			return; // Do not display it the usual way
		}

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
