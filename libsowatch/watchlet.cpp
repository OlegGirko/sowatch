#include "watchlet.h"
#include "watchserver.h"

using namespace sowatch;

Watchlet::Watchlet(WatchServer *server, const QString& id) :
	QObject(server), _id(id), _active(false), _server(server)
{
	_server->registerWatchlet(this);
}

Watchlet::~Watchlet()
{

}

WatchServer* Watchlet::server()
{
	return _server;
}

Watch* Watchlet::watch()
{
	return _server->watch();
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
