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
}

Watch* WatchServer::watch()
{
	return _watch;
}

void WatchServer::addProvider(NotificationProvider *provider)
{
	provider->setParent(this);

	connect(provider, SIGNAL(notification(Notification)), SLOT(notificationEmitted(Notification)));
	connect(provider, SIGNAL(unreadCountChanged(Notification::Type)), SLOT(unreadCountUpdated(Notification::Type)));

	_providers.append(provider);
}

void WatchServer::runWatchlet(const QString& id)
{
	if (_currentWatchlet) {
		closeWatchlet();
	}
	_currentWatchlet = _watchlets[id];
	if (_watch->isConnected()) {
		_currentWatchlet->activate();
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

void WatchServer::watchConnected()
{
	if (_currentWatchlet) {
		_currentWatchlet->activate();
	}
}

void WatchServer::watchDisconnected()
{
	if (_currentWatchlet) {
		_currentWatchlet->deactivate();
	}
}

void WatchServer::notificationEmitted(const Notification &notification)
{
	// TODO app loses button focus...
	_watch->showNotification(notification);
}

void WatchServer::unreadCountUpdated(Notification::Type type)
{
	uint count = 0;
	foreach(NotificationProvider* provider, _providers)
	{
		count += provider->getCount(type);
	}
	_watch->updateNotificationCount(type, count);
}
