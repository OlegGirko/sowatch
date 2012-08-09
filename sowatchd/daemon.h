#ifndef WATCHDAEMON_H
#define WATCHDAEMON_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSignalMapper>

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

signals:
	void WatchStatusChanged(const QString &watch, const QString &status);

private:
	Registry* _registry;
	ConfigKey* _config;
	ConfigKey* _watches_list;
	QMap<QString, WatchServer*> _servers;
	QSignalMapper *_status_mapper;

	void startWatch(const QString& name);
	void stopWatch(const QString& name);

private slots:
	void startEnabledWatches();
	void handleSettingsChanged(const QString& subkey);
	void handleWatchStatusChange(const QString& watch);
};

}

#endif // WATCHDAEMON_H
