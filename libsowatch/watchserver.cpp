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

void WatchServer::runWatchlet(const QString& id)
{
	if (_currentWatchlet) {
		closeWatchlet();
	}
	_currentWatchlet = watchlets[id];
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
	watchlets[watchlet->id()] = watchlet;
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
