#include "watchhandler.h"

using namespace sowatch;

WatchHandler::WatchHandler(ConfigKey *config, QObject *parent)
    : QObject(parent),
      _config(config->getSubkey("", this))
{
	Registry *registry = Registry::registry();

	qDebug() << "Starting watch handler on" << _config->key();

	connect(_config, SIGNAL(subkeyChanged(QString)),
	        SLOT(handleConfigSubkeyChanged(QString)));

	// Connect to the registry in case plugins are unloaded
	connect(registry, SIGNAL(driverUnloaded(QString)),
	        SLOT(handleDriverUnloaded(QString)));
	connect(registry, SIGNAL(watchletLoaded(QString)),
	        SLOT(updateWatchlets()));
	connect(registry, SIGNAL(watchletUnloaded(QString)),
	        SLOT(handleWatchletUnloaded(QString)));
	connect(registry, SIGNAL(notificationProviderLoaded(QString)),
	        SLOT(updateProviders()));
	connect(registry, SIGNAL(notificationProviderUnloaded(QString)),
	        SLOT(handleProviderUnloaded(QString)));

	// Get the watch driver
	const QString driver = _config->value("driver").toString();
	if (driver.isEmpty()) {
		qWarning() << "Watch" << _config->value("name") << "has no driver setting";
		return;
	}


	WatchPluginInterface *watchPlugin = registry->getWatchPlugin(driver);
	if (!watchPlugin) {
		qWarning() << "Invalid driver" << driver;
		return;
	}

	// Create the watch object from the plugin
	_watch = watchPlugin->getWatch(driver, _config, this);
	if (!_watch) {
		qWarning() << "Driver" << driver << "failed to initiate watch";
		return;
	}

	// Setup watch status connections
	connect(_watch, SIGNAL(connected()),
	        SIGNAL(statusChanged()));
	connect(_watch, SIGNAL(disconnected()),
	        SIGNAL(statusChanged()));

	// Now create the UI server
	_server = new WatchServer(_watch, this);

	// Configure the server
	_server->setNextWatchletButton(_config->value("next-watchlet-button").toString());

	updateProviders();
	updateWatchlets();
}

QString WatchHandler::status() const
{
	if (_watch && _watch->isConnected()) {
		return "connected";
	} else if (_config->value("enable").toBool()) {
		return "enabled";
	} else {
		return "disabled";
	}
}

Watchlet* WatchHandler::createWatchlet(const QString &id)
{
	Registry *registry = Registry::registry();
	WatchletPluginInterface *plugin = registry->getWatchletPlugin(id);
	if (!plugin) {
		qWarning() << "Unknown watchlet" << id;
		return 0;
	}

	ConfigKey *subconfig = _config->getSubkey(id);
	Watchlet* watchlet = plugin->getWatchlet(id, subconfig, _server);
	delete subconfig;

	return watchlet;
}

void WatchHandler::deleteWatchletAt(int index)
{
	const QString id = _watchlet_order[index];
	Watchlet *watchlet = _watchlets[id];

	_server->removeWatchlet(watchlet);
	_watchlet_order.removeAt(index);
	_watchlets.remove(id);

	delete watchlet;
}

void WatchHandler::updateWatchlets()
{
	if (!_server) return;

	QStringList newWatchlets = _config->value("watchlets").toStringList();

	// Try to do one operation at the time
	// (e.g. move a watchlet or add/remove it).
	// Find the first difference
	int i;
	for (i = 0; i < newWatchlets.size(); i++) {
		// Precondition: newWatchlets and curWatchlets are equal in range 0..i-1
		if (i >= _watchlet_order.size()) {
			// We need to add this watchlet
			const QString id = newWatchlets[i];
			Watchlet *watchlet = createWatchlet(id);
			if (watchlet) {
				_watchlet_order << id;
				_watchlets[id] = watchlet;
				_server->addWatchlet(watchlet);
			} else {
				qWarning() << "Failed to load watchlet" << id;
				newWatchlets.removeAt(i);
				i--; // Retry
			}
		} else if (newWatchlets[i] != _watchlet_order[i]) {
			// Let's find out if this watchlet has been moved, or removed.
			const QString id = _watchlet_order[i];
			int j;
			for (j = i; j < newWatchlets.size(); j++) {
				if (id == newWatchlets[j]) {
					break; // Found
				}
			}
			if (j == _watchlet_order.size()) {
				// It was not found, so it has been removed
				deleteWatchletAt(i);
			} else {
				// It has been found at index j, it needs to moved.
				_watchlet_order.move(i, j);
				_server->moveWatchlet(_watchlets[id], j);
			}
		}
	}
	while (i < _watchlet_order.size()) {
		// These watchlets are to be unloaded
		deleteWatchletAt(i);
	}

	Q_ASSERT(newWatchlets == _watchlet_order);

	qDebug() << "New watchlet order: " << _watchlet_order;
}

void WatchHandler::updateProviders()
{
	Registry *registry = Registry::registry();

	if (!_server) return;

	QSet<QString> curProviders = _providers.keys().toSet();
	QSet<QString> newProviders = _config->value("providers").toStringList().toSet();
	QSet<QString> removed = curProviders - newProviders;
	QSet<QString> added = newProviders - curProviders;

	qDebug() << "Providers to remove: " << removed;
	qDebug() << "Providers to add: " << added;

	foreach (const QString& s, removed) {
		NotificationProvider *provider = _providers[s];
		_server->removeProvider(provider);
		_providers.remove(s);
		delete provider;
	}

	foreach (const QString& s, added) {
		NotificationPluginInterface *plugin = registry->getNotificationPlugin(s);
		if (!plugin) {
			qWarning() << "Unknown notification provider" << s;
			continue;
		}
		ConfigKey *subconfig = _config->getSubkey(s);
		NotificationProvider *provider = plugin->getProvider(s, subconfig, _server);
		_server->addProvider(provider);
		_providers[s] = provider;
		delete subconfig;
	}

	qDebug() << "Providers reloaded: " << _providers.keys();
}

void WatchHandler::handleConfigSubkeyChanged(const QString &subkey)
{
	if (subkey == "watchlets") {
		qDebug() << "Watchlets list changed";
		updateWatchlets();
	} else if (subkey == "providers") {
		qDebug() << "Providers list changed";
		updateProviders();
	} else if (subkey == "next-watchlet-button" && _server) {
		_server->setNextWatchletButton(_config->value("next-watchlet-button").toString());
	}
}

void WatchHandler::handleDriverUnloaded(const QString &id)
{
	if (id == _config->value("driver").toString()) {
		// Emergency disconnection!
		qWarning("Unloading driver of active watch!");
		if (_server) {
			delete _server;
			_server = 0;
		}
		if (_watch) {
			delete _watch;
			_watch = 0;
		}
		emit statusChanged();
	}
}

void WatchHandler::handleWatchletUnloaded(const QString &id)
{
	if (_watchlets.contains(id)) {
		qDebug() << "Unloading watchlet" << id << "from watch";
		Watchlet* watchlet = _watchlets[id];
		_server->removeWatchlet(watchlet);
		_watchlets.remove(id);
		delete watchlet;
	}
}

void WatchHandler::handleProviderUnloaded(const QString &id)
{
	if (_providers.contains(id)) {
		qDebug() << "Unloading provider" << id << "from watch";
		NotificationProvider *provider = _providers[id];
		_server->removeProvider(provider);
		_providers.remove(id);
		delete provider;
	}
}
