#include <QtCore/QDebug>
#include "watch.h"
#include "declarativewatchwrapper.h"

using namespace sowatch;

DeclarativeWatchWrapper::DeclarativeWatchWrapper(Watch* watch, QObject *parent) :
	QObject(parent), _watch(watch), _active(false)
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

void DeclarativeWatchWrapper::activate()
{
	if (!_active) {
		connect(_watch, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
		connect(_watch, SIGNAL(buttonReleased(int)), this, SIGNAL(buttonReleased(int)));
		_active = true;
		emit activeChanged();
	}
}

void DeclarativeWatchWrapper::deactivate()
{
	if (_active) {
		disconnect(this, SIGNAL(buttonPressed(int)));
		disconnect(this, SIGNAL(buttonReleased(int)));
		_active = false;
		emit activeChanged();
	}
}

