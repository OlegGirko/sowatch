#ifndef WATCHSERVER_H
#define WATCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include "sowatch_global.h"

namespace sowatch
{

class Watch;
class Watchlet;
class Notification;

class SOWATCH_EXPORT WatchServer : public QObject
{
    Q_OBJECT
	Q_PROPERTY(Watch* watch READ watch CONSTANT)

public:
	explicit WatchServer(Watch* watch, QObject* parent = 0);

	Watch* watch();

	void runWatchlet(const QString& id);
	void closeWatchlet();

signals:

public slots:
	void notification(const Notification& n);

protected:
	Watch* _watch;
	Watchlet* _currentWatchlet;

	QMap<QString, Watchlet*> watchlets;

	void registerWatchlet(Watchlet *watchlet);

protected slots:
	void watchConnected();
	void watchDisconnected();

friend class Watchlet;
};

}

#endif // WATCHSERVER_H
