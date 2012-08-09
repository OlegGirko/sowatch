#include <QtCore/QDebug>
#include <sowatch.h>
#include "daemon.h"

using namespace sowatch;

Daemon::Daemon(QObject *parent) :
	QObject(parent),
	_registry(Registry::registry()),
	_config(new GConfKey("/apps/sowatch", this)),
	_watches_list(_config->getSubkey("watches", this)),
	_status_mapper(new QSignalMapper(this))
{
	connect(_config, SIGNAL(subkeyChanged(QString)),
	        SLOT(handleSettingsChanged(QString)));
	connect(_status_mapper, SIGNAL(mapped(QString)),
	        SLOT(handleWatchStatusChange(QString)));

	startEnabledWatches();
}

QString Daemon::getWatchStatus(const QString &name)
{
	if (_watches.contains(name)) {
		return _watches[name]->status();
	} else {
		return QLatin1String("disabled");
	}
}

void Daemon::terminate()
{
	QApplication::quit();
}

void Daemon::startWatch(const QString &name)
{
	qDebug() << "Starting watch" << name;
	QScopedPointer<ConfigKey> watchConfig(_config->getSubkey(name));
	WatchHandler *handler = new WatchHandler(watchConfig.data(), this);
	_status_mapper->setMapping(handler, name);
	_watches[name] = handler;
	handleWatchStatusChange(name);
	connect(handler, SIGNAL(statusChanged()),
	        _status_mapper, SLOT(map()));
}

void Daemon::stopWatch(const QString &name)
{
	qDebug() << "Stopping watch" << name;
	WatchHandler *handler = _watches[name];
	_watches.remove(name);
	_status_mapper->removeMappings(handler);
	delete handler;
	handleWatchStatusChange(name);
}

void Daemon::startEnabledWatches()
{
	QStringList watches = _watches_list->value().toStringList();
	QSet<QString> startedWatches = _watches.keys().toSet();

	QSet<QString> removed = startedWatches - watches.toSet();
	// Those watches have been entirely removed from the list, not disabled first
	foreach (const QString& s, removed) {
		stopWatch(s);
	}

	foreach (const QString& s, watches) {
		bool enabled_in_config = _config->value(s + "/enable").toBool();
		bool currently_started = _watches.contains(s);

		if (enabled_in_config && !currently_started) {
			startWatch(s);
		} else if (currently_started && !enabled_in_config) {
			stopWatch(s);
		}
	}
}

void Daemon::handleSettingsChanged(const QString &subkey)
{
	qDebug() << "Daemon settings changed" << subkey;
	static QRegExp enabled_key_pattern("([^/]+)/enable");
	if (enabled_key_pattern.exactMatch(subkey)) {
		QString watchName = enabled_key_pattern.cap(1);
		bool enabled_in_config = _config->value(subkey).toBool();
		bool currently_started = _watches.contains(watchName);

		if (enabled_in_config && !currently_started) {
			startWatch(watchName);
		} else if (currently_started && !enabled_in_config) {
			stopWatch(watchName);
		}
	} else if (subkey == "watches") {
		startEnabledWatches();
	}
}

void Daemon::handleWatchStatusChange(const QString &name)
{
	if (_watches.contains(name)) {
		emit WatchStatusChanged(name, _watches[name]->status());
	} else if (_watches_list->value().toStringList().contains(name)) {
		emit WatchStatusChanged(name, QLatin1String("disabled"));
	} else {
		emit WatchStatusChanged(name, QLatin1String("unconfigured"));
	}
}
