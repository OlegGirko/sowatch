#ifndef WATCHDAEMON_H
#define WATCHDAEMON_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include <sowatch.h>

namespace sowatch
{

class Daemon : public QObject
{
	Q_OBJECT
public:
	explicit Daemon(QObject *parent = 0);

	Q_INVOKABLE QString getWatchStatus(const QString& name);

public slots:
	void terminate();

private:
	Registry* _registry;
	ConfigKey* _settings;
	QMap<QString, WatchServer*> _servers;

	void startWatch(const QString& name);
	void stopWatch(const QString& name);

private slots:
	void settingsChanged(const QString& subkey);
};

}

#endif // WATCHDAEMON_H
