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
	// TODO: Figure a better way for this; QAbstractListModel, etc.
	QList<Notification*> nl = _server->liveNotifications();
	QList<QObject*> ol;
	foreach (Notification* n, nl) {
		QObject * o = n;
		ol.append(o);
	}
	qDebug() << "notifications declarative: " << ol;
	return ol;
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
		connect(_watch, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
		connect(_watch, SIGNAL(buttonReleased(int)), this, SIGNAL(buttonReleased(int)));
		_active = true;
		emit activeChanged();
		// Since a notification currently causes the active watchlet to be deactivated,
		// we can assume notifications only change when we are deactivated.
		emit notificationsChanged();
	}
}

void DeclarativeWatchWrapper::deactivate()
{
	if (_active) {
		disconnect(_watch, 0, this, 0);
		_active = false;
		emit activeChanged();
	}
}

