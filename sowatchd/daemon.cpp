#include <QtCore/QDebug>
#include <sowatch.h>
#include "daemon.h"

using namespace sowatch;

Daemon::Daemon(QObject *parent) :
	QObject(parent),
	_registry(Registry::registry())
{
	initWatches();
}

void Daemon::initWatches()
{
	QSettings settings;
	int size = settings.beginReadArray("watches");

	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString driver = settings.value("driver").toString().toLower();
		WatchPluginInterface *plugin = _registry->getWatchPlugin(driver);
		if (plugin) {
			Watch *watch = plugin->getWatch(driver, settings, this);
			if (watch) {
				initWatch(watch, settings);
			} else {
				qWarning() << "Driver" << driver << "refused to getWatch";
			}
		} else {
			qWarning() << "Invalid driver" << driver;
		}
	}

	settings.endArray();
	qDebug() << "handling" << _servers.size() << "watches";
}

void Daemon::initWatch(Watch* watch, QSettings& settings)
{
	int size;

	// Create the server
	WatchServer* server = new WatchServer(watch, this);
	_servers.append(server);

	// Configure the server
	server->setNextWatchletButton(settings.value("nextWatchletButton").toString());

	// Initialize providers
	size = settings.beginReadArray("notifications");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString id = settings.value("provider").toString().toLower();
		NotificationPluginInterface *plugin = _registry->getNotificationPlugin(id);
		if (plugin) {
			NotificationProvider *provider = plugin->getProvider(id, settings, server);
			server->addProvider(provider);
		} else {
			qWarning() << "Unknown notification provider" << id;
		}
	}
	settings.endArray();

	// Initialize watchlets
	size = settings.beginReadArray("watchlets");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString id = settings.value("id").toString().toLower();
		WatchletPluginInterface *plugin = _registry->getWatchletPlugin(id);
		if (plugin) {
			plugin->getWatchlet(id, settings, server);
			// Watchlets are associated to server via parent-child relationship.
		} else {
			qWarning() << "Unknown watchlet" << id;
		}
	}
	settings.endArray();
}
