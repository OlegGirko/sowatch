#include <QtCore/QDebug>
#include <QtCore/QPluginLoader>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <sowatch.h>
#include "daemon.h"

using namespace sowatch;

Daemon::Daemon(QObject *parent) :
    QObject(parent)
{
	loadDrivers();
	loadProviders();
	loadWatchlets();
	initWatches();
}

void Daemon::loadDrivers()
{
	QDir dir(SOWATCH_DRIVERS_DIR);
	foreach (QString file, dir.entryList(QDir::Files)) {
#if defined(Q_OS_UNIX)
		// Temporary workaround for QtC deploy plugin issues
		if (!file.endsWith(".so")) continue;
#endif
		QPluginLoader loader(dir.absoluteFilePath(file));
		QObject *pluginObj = loader.instance();
		if (pluginObj) {
			WatchPluginInterface *plugin = qobject_cast<WatchPluginInterface*>(pluginObj);
			if (plugin) {
				QStringList drivers = plugin->drivers();
				foreach (const QString& driver, drivers) {
					_drivers[driver] = plugin;
				}
			} else {
				qWarning() << "Invalid plugin" << file;
				loader.unload();
			}
		} else {
			qWarning() << "Invalid plugin" << file << loader.errorString();
			loader.unload();
		}
	}

	qDebug() << "loaded drivers" << _drivers.keys();
}

void Daemon::loadProviders()
{
	QDir dir(SOWATCH_NOTIFICATIONS_DIR);
	foreach (QString file, dir.entryList(QDir::Files)) {
#if defined(Q_OS_UNIX)
		// Temporary workaround for QtC deploy plugin issues
		if (!file.endsWith(".so")) continue;
#endif
		QPluginLoader loader(dir.absoluteFilePath(file));
		QObject *pluginObj = loader.instance();
		if (pluginObj) {
			NotificationPluginInterface *plugin = qobject_cast<NotificationPluginInterface*>(pluginObj);
			if (plugin) {
				QStringList providers = plugin->providers();
				foreach (const QString& provider, providers) {
					_providers[provider] = plugin;
				}
			} else {
				qWarning() << "Invalid plugin" << file;
				loader.unload();
			}
		} else {
			qWarning() << "Invalid plugin" << file << loader.errorString();
			loader.unload();
		}
	}

	qDebug() << "loaded providers" << _providers.keys();
}

void Daemon::initWatches()
{
	QSettings settings;
	int size = settings.beginReadArray("watches");

	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString driver = settings.value("driver").toString().toLower();
		WatchPluginInterface *plugin = _drivers[driver];
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
		NotificationPluginInterface *plugin = _providers[id];
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
		WatchletPluginInterface *plugin = _watchlets[id];
		if (plugin) {
			plugin->getWatchlet(id, settings, server);
			// Watchlets are associated to server via parent-child relationship.
		} else {
			qWarning() << "Unknown watchlet" << id;
		}
	}
	settings.endArray();
}

void Daemon::loadWatchlets()
{
	QDir dir(SOWATCH_WATCHLETS_DIR);
	foreach (QString file, dir.entryList(QDir::Files)) {
#if defined(Q_OS_UNIX)
		// Temporary workaround for QtC deploy plugin issues
		if (!file.endsWith(".so")) continue;
#endif
		QPluginLoader loader(dir.absoluteFilePath(file));
		QObject *pluginObj = loader.instance();
		if (pluginObj) {
			WatchletPluginInterface *plugin = qobject_cast<WatchletPluginInterface*>(pluginObj);
			if (plugin) {
				QStringList watchlets = plugin->watchlets();
				foreach (const QString& watchlet, watchlets) {
					_watchlets[watchlet] = plugin;
				}
			} else {
				qWarning() << "Invalid plugin" << file;
				loader.unload();
			}
		} else {
			qWarning() << "Invalid plugin" << file << loader.errorString();
			loader.unload();
		}
	}

	qDebug() << "loaded watchlets" << _watchlets.keys();
}
