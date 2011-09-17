#ifndef WATCHSERVER_H
#define WATCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QMap>

namespace sowatch
{

class Watch;
class Watchlet;

class WatchServer : public QObject
{
    Q_OBJECT
	Q_PROPERTY(Watch* watch READ watch)
public:
	explicit WatchServer(Watch* watch, QObject* parent = 0);

	Watch* watch();

	void runWatchlet(const QString& id);
	void closeWatchlet();

signals:

public slots:

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
