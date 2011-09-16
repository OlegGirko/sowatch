#include "watchlet.h"
#include "watchserver.h"

using namespace sowatch;

Watchlet::Watchlet(WatchServer *server, const QString& id) :
	QObject(server), _id(id), _server(server)
{
	_server->registerWatchlet(this);
}

WatchServer* Watchlet::server()
{
	return _server;
}

Watch* Watchlet::watch()
{
	return _server->watch();
}

QString Watchlet::id()
{
	return _id;
}
