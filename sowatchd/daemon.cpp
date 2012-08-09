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
	if (_servers.contains(name)) {
		WatchServer* server = _servers[name];
		Watch* watch = server->watch();
		if (watch->isConnected()) {
			return QLatin1String("connected");
		} else {
			return QLatin1String("enabled");
		}
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
	QScopedPointer<ConfigKey> watchSettings(_config->getSubkey(name));

	const QString driver = watchSettings->value("driver").toString().toLower();
	if (driver.isEmpty()) {
		qWarning() << "Watch" << name << "has no driver setting";
		return;
	}

	WatchPluginInterface *watchPlugin = _registry->getWatchPlugin(driver);
	if (!watchPlugin) {
		qWarning() << "Invalid driver" << driver;
		return;
	}

	// Create the watch object from the plugin
	Watch *watch = watchPlugin->getWatch(driver, watchSettings.data(), this);
	if (!watch) {
		qWarning() << "Driver" << driver << "failed to initiate watch";
	}

	// Create the server
	WatchServer* server = new WatchServer(watch, this);
	_servers[name] = server;

	handleWatchStatusChange(name);

	// Connect watch status signals
	_status_mapper->setMapping(watch, name);
	connect(watch, SIGNAL(connected()),
	        _status_mapper, SLOT(map()));
	connect(watch, SIGNAL(disconnected()),
	        _status_mapper, SLOT(map()));

	// Configure the server
	server->setNextWatchletButton(watchSettings->value("next-watchlet-button").toString());

	// Initialize providers
	QStringList list;
	list = watchSettings->value("active-notifications").toStringList();
	foreach (const QString& s, list) {
		QScopedPointer<ConfigKey> settings(watchSettings->getSubkey(s));
		QString id = settings->value("id").toString().toLower();
		NotificationPluginInterface *plugin = _registry->getNotificationPlugin(id);
		if (plugin) {
			NotificationProvider *provider = plugin->getProvider(id, settings.data(), server);
			server->addProvider(provider);
		} else {
			qWarning() << "Unknown notification provider" << id;
		}
	}

	// Initialize watchlets
	list = watchSettings->value("active-watchlets").toStringList();
	foreach (const QString& s, list) {
		QScopedPointer<ConfigKey> settings(watchSettings->getSubkey(s));
		QString id = settings->value("id").toString().toLower();
		WatchletPluginInterface *plugin = _registry->getWatchletPlugin(id);
		if (plugin) {
			Watchlet *watchlet = plugin->getWatchlet(id, settings.data(), server);
			server->addWatchlet(watchlet);
		} else {
			qWarning() << "Unknown watchlet" << id;
		}
	}
}

void Daemon::stopWatch(const QString &name)
{
	qDebug() << "Stopping watch" << name;
	WatchServer* server = _servers[name];
	Watch* watch = server->watch();
	server->deleteLater();
	watch->deleteLater();
	_servers.remove(name);
	handleWatchStatusChange(name);
}

void Daemon::startEnabledWatches()
{
	QStringList watches = _watches_list->value().toStringList();
	QSet<QString> startedWatches = _servers.keys().toSet();

	QSet<QString> removed = startedWatches - watches.toSet();
	// Those watches have been entirely removed from the list, not disabled first
	foreach (const QString& s, removed) {
		stopWatch(s);
	}

	foreach (const QString& s, watches) {
		bool enabled_in_config = _config->value(s + "/enable").toBool();
		bool currently_started = _servers.contains(s);

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
	static QRegExp enabled_key_pattern("^([^/])/enable$");
	if (enabled_key_pattern.exactMatch(subkey)) {
		QStringList watches = _watches_list->value().toStringList();
		QString watchName = enabled_key_pattern.cap(1);
		bool enabled_in_config = _config->value(subkey).toBool();
		bool currently_started = _servers.contains(watchName);

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
	if (_servers.contains(name)) {
		WatchServer* server = _servers[name];
		Watch* watch = server->watch();
		if (watch->isConnected()) {
			emit WatchStatusChanged(name, QLatin1String("connected"));
		} else {
			emit WatchStatusChanged(name, QLatin1String("enabled"));
		}
	} else if (_watches_list->value().toStringList().contains(name)) {
		emit WatchStatusChanged(name, QLatin1String("disabled"));
	} else {
		emit WatchStatusChanged(name, QLatin1String("unconfigured"));
	}
}
