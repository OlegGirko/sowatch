#ifndef WATCHDAEMON_H
#define WATCHDAEMON_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSettings>

#include <sowatch.h>

namespace sowatch
{

class Daemon : public QObject
{
	Q_OBJECT
public:
	explicit Daemon(QObject *parent = 0);

protected:
	Registry* _registry;
	QList<WatchServer*> _servers;

	void initWatches();
	void initWatch(Watch* watch, QSettings& settings);
};

}

#endif // WATCHDAEMON_H
