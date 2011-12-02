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

QList<QObject*> DeclarativeWatchWrapper::notifications() const
{
	// TODO: Figure out a better way for this; QAbstractListModel, etc.
	QList<Notification*> nl = _server->liveNotifications();
	QList<QObject*> ol;
	foreach (Notification* n, nl) {
		QObject * o = n;
		ol.append(o);
	}
	qDebug() << "notifications to declarative: " << ol;
	return ol;
}

void DeclarativeWatchWrapper::useButton(int button)
{
	if (!_usedButtons.contains(button)) {
		_usedButtons.insert(button);
		if (_active) {
			_watch->grabButton(button);
		}
	}
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

		// Grab all of the buttons used by this watchlet
		foreach (int button, _usedButtons) {
			_watch->grabButton(button);
		}

		// Forward the button signals
		connect(_watch, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
		connect(_watch, SIGNAL(buttonReleased(int)), this, SIGNAL(buttonReleased(int)));

		// Emit the active signal
		emit activeChanged();

		// Since a notification currently causes the active watchlet to be deactivated,
		// we can assume the notifications list only changes when we are deactivated.
		emit notificationsChanged();
	}
}

void DeclarativeWatchWrapper::deactivate()
{
	if (_active) {
		_active = false;

		// Stop forwarding button presses
		disconnect(_watch, 0, this, 0);

		// Ungrab all the buttons used by this watchlet
		foreach (int button, _usedButtons) {
			_watch->ungrabButton(button);
		}

		// Emit the deactivated signal
		emit activeChanged();
	}
}

