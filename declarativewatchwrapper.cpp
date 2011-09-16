#include <QtCore/QDebug>
#include "watch.h"
#include "declarativewatchwrapper.h"

using namespace sowatch;

DeclarativeWatchWrapper::DeclarativeWatchWrapper(Watch* watch, QObject *parent) :
	QObject(parent), _watch(watch)
{

}

QString DeclarativeWatchWrapper::model() const
{
	return _watch->model();
}

void DeclarativeWatchWrapper::activate()
{
	connect(_watch, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
	connect(_watch, SIGNAL(buttonReleased(int)), this, SIGNAL(buttonReleased(int)));
}

void DeclarativeWatchWrapper::deactivate()
{
	disconnect(this, SIGNAL(buttonPressed(int)));
	disconnect(this, SIGNAL(buttonReleased(int)));
}

