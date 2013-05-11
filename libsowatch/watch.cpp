#include "watch.h"
#include "watchpaintengine.h"

using namespace sowatch;

Watch::Watch(QObject* parent) :
	QObject(parent)
{

}

Watch::~Watch()
{

}

void Watch::vibrate(int msecs)
{
	/* The default implementation does nothing. */
	Q_UNUSED(msecs);
}

void Watch::setWatchletsModel(WatchletsModel *model)
{
	Q_UNUSED(model);
}

void Watch::setNotificationsModel(NotificationsModel *model)
{
	Q_UNUSED(model);
}
