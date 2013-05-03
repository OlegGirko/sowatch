#include <QtCore/QDebug>
#include "watchserver.h"
#include "watch.h"
#include "notification.h"
#include "declarativewatchwrapper.h"

using namespace sowatch;

DeclarativeWatchWrapper::DeclarativeWatchWrapper(WatchServer* server, Watch* watch, QObject* parent) :
	QObject(parent), _server(server), _watch(watch),
	_active(false)
{

}

QString DeclarativeWatchWrapper::model() const
{
	return _watch->model();
}

bool DeclarativeWatchWrapper::active() const
{
	return _active;
}

void DeclarativeWatchWrapper::vibrate(int msecs)
{
	if (_active) {
		_watch->vibrate(msecs);
	}
}

void DeclarativeWatchWrapper::activate()
{
	if (!_active) {
		_active = true;

		// Forward the button signals
		connect(_watch, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
		connect(_watch, SIGNAL(buttonReleased(int)), this, SIGNAL(buttonReleased(int)));

		// Emit the active signal
		emit activeChanged();
	}
}

void DeclarativeWatchWrapper::deactivate()
{
	if (_active) {
		_active = false;

		// Stop forwarding button presses
		disconnect(_watch, 0, this, 0);

		// Emit the deactivated signal
		emit activeChanged();
	}
}
