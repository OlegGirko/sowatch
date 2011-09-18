#ifndef WATCHDAEMON_H
#define WATCHDAEMON_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>

#include <sowatch.h>

namespace sowatch
{

class Daemon : public QObject
{
    Q_OBJECT
public:
	explicit Daemon(QObject *parent = 0);

protected:
	QMap<QString, WatchPluginInterface*> _drivers;
	QList<Watch*> _watches;

	void loadDrivers();
	void loadWatches();
	void loadWatchlets();
};

}

#endif // WATCHDAEMON_H
