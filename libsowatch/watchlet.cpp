#include "watchlet.h"
#include "watch.h"

using namespace sowatch;

Watchlet::Watchlet(Watch *watch, const QString& id) :
	QObject(watch), _id(id), _active(false), _watch(watch)
{

}

Watchlet::~Watchlet()
{

}

const Watch* Watchlet::watch() const
{
	return _watch;
}

Watch* Watchlet::watch()
{
	return _watch;
}

QString Watchlet::id() const
{
	return _id;
}

bool Watchlet::isActive() const
{
	return _active;
}

void Watchlet::activate()
{
	_active = true;
	emit activeChanged();
	emit activated();
}

void Watchlet::deactivate()
{
	_active = false;
	emit activeChanged();
	emit deactivated();
}

void Watchlet::setNotificationsModel(NotificationsModel *model)
{
	Q_UNUSED(model);
}

bool Watchlet::handlesNotification(Notification *notification) const
{
	Q_UNUSED(notification);
	return false;
}

void Watchlet::openNotification(Notification *notification)
{
	Q_UNUSED(notification);
	qDebug() << "Watchlet" << _id << "does not override openNotification()";
}
