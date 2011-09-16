#ifndef WATCHLET_H
#define WATCHLET_H

#include <QtCore/QObject>

namespace sowatch
{

class Watch;
class WatchServer;

class Watchlet : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString id READ id)

public:
	explicit Watchlet(WatchServer *server, const QString& id);

	WatchServer* server();
	Watch* watch();

	QString id();

protected:
	virtual void activate() = 0;
	virtual void deactivate() = 0;

private:
	QString _id;
	WatchServer* _server;

friend class WatchServer;
};

}

#endif // WATCHLET_H
