#include <QtCore/QDebug>
#include <QtCore/QPluginLoader>
#include <QtCore/QSettings>
#include <QtCore/QDir>

#include "watchplugininterface.h"
#include "notificationplugininterface.h"
#include "watchletplugininterface.h"
#include "registry.h"

using namespace sowatch;

Registry* Registry::singleRegistry = 0;

Registry* Registry::registry()
{
	if (!singleRegistry) {
		singleRegistry = new Registry();
	}

	return singleRegistry;
}

Registry::~Registry()
{
}

Registry::Registry()
	: _watcher(new QFileSystemWatcher(this))
{
	_watcher->addPath(SOWATCH_DRIVERS_DIR);
	_watcher->addPath(SOWATCH_NOTIFICATIONS_DIR);
	_watcher->addPath(SOWATCH_WATCHLETS_DIR);

	loadDrivers();
	loadNotificationProviders();
	loadWatchlets();

	connect(_watcher, SIGNAL(directoryChanged(QString)),
			this, SLOT(handlePluginDirectoryChanged(QString)));
	connect(_watcher, SIGNAL(fileChanged(QString)),
			this, SLOT(handlePluginFileChanged(QString)));
}

void Registry::loadDrivers()
{
	QDir dir(SOWATCH_DRIVERS_DIR);
	foreach (QString entry, dir.entryList(QDir::Files)) {
		QString file = dir.absoluteFilePath(entry);
		loadDriver(file);
	}

	qDebug() << "loaded drivers" << _driverIds.keys();
}

void Registry::loadNotificationProviders()
{
	QDir dir(SOWATCH_NOTIFICATIONS_DIR);
	foreach (QString entry, dir.entryList(QDir::Files)) {
		QString file = dir.absoluteFilePath(entry);
		loadNotificationProvider(file);
	}

	qDebug() << "loaded notification providers" << _providerIds.keys();
}

void Registry::loadWatchlets()
{
	QDir dir(SOWATCH_WATCHLETS_DIR);
	foreach (QString entry, dir.entryList(QDir::Files)) {
		QString file = dir.absoluteFilePath(entry);
		loadWatchlet(file);
	}

	qDebug() << "loaded watchlets" << _watchletIds.keys();
}

void Registry::loadDriver(const QString &file)
{
	QPluginLoader* loader = new QPluginLoader(file, this);
	QObject *pluginObj = loader->instance();
	if (pluginObj) {
		WatchPluginInterface *plugin = qobject_cast<WatchPluginInterface*>(pluginObj);
		if (plugin) {
			_driverFiles[file] = loader;
			_drivers += plugin;
			QStringList drivers = plugin->drivers();
			foreach (const QString& driver, drivers) {
				_driverIds[driver] = plugin;
				emit driverLoaded(driver);
			}
			_watcher->addPath(file);
		} else {
			qWarning() << "Invalid plugin" << file;
			loader->unload();
			delete loader;
		}
	} else {
		qWarning() << "Invalid plugin" << file << loader->errorString();
		loader->unload();
		delete loader;
	}
}

void Registry::loadNotificationProvider(const QString &file)
{
	QPluginLoader* loader = new QPluginLoader(file, this);
	QObject *pluginObj = loader->instance();
	if (pluginObj) {
		NotificationPluginInterface *plugin = qobject_cast<NotificationPluginInterface*>(pluginObj);
		if (plugin) {
			_providerFiles[file] = loader;
			_providers += plugin;
			QStringList providers = plugin->providers();
			foreach (const QString& provider, providers) {
				_providerIds[provider] = plugin;
				emit notificationProviderLoaded(provider);
			}
			_watcher->addPath(file);
		} else {
			qWarning() << "Invalid plugin" << file;
			loader->unload();
			delete loader;
		}
	} else {
		qWarning() << "Invalid plugin" << file << loader->errorString();
		loader->unload();
		delete loader;
	}
}

void Registry::loadWatchlet(const QString &file)
{
	QPluginLoader* loader = new QPluginLoader(file, this);
	QObject *pluginObj = loader->instance();
	if (pluginObj) {
		WatchletPluginInterface *plugin = qobject_cast<WatchletPluginInterface*>(pluginObj);
		if (plugin) {
			_watchletFiles[file] = loader;
			_watchlets += plugin;
			QStringList watchlets = plugin->watchlets();
			foreach (const QString& watchlet, watchlets) {
				_watchletIds[watchlet] = plugin;
				emit watchletLoaded(watchlet);
			}
			_watcher->addPath(file);
		} else {
			qWarning() << "Invalid plugin" << file;
			loader->unload();
			delete loader;
		}
	} else {
		qWarning() << "Invalid plugin" << file << loader->errorString();
		loader->unload();
		delete loader;
	}
}

void Registry::unloadDriver(QPluginLoader *loader)
{
	QString file = loader->fileName();

	WatchPluginInterface *plugin = qobject_cast<WatchPluginInterface*>(loader->instance());
	QStringList drivers = plugin->drivers();

	foreach (const QString& driver, drivers) {
		emit driverUnloaded(driver);
		_driverIds.remove(driver);
	}

	_drivers.removeAll(plugin);
	_driverFiles.remove(file);
	_watcher->removePath(file);

	qDebug() << "Now unloading" << file;
	if (!loader->unload()) {
		qWarning() << "Could not unload plugin" << file;
	}

	delete loader;
}

void Registry::unloadNotificationProvider(QPluginLoader *loader)
{
	QString file = loader->fileName();
	NotificationPluginInterface *plugin = qobject_cast<NotificationPluginInterface*>(loader->instance());
	QStringList providers = plugin->providers();

	foreach (const QString& provider, providers) {
		emit notificationProviderUnloaded(provider);
		_providerIds.remove(provider);
	}

	_providers.removeAll(plugin);
	_providerFiles.remove(file);
	_watcher->removePath(file);

	qDebug() << "Now unloading" << file;
	if (!loader->unload()) {
		qWarning() << "Could not unload plugin" << file;
	}

	delete loader;
}

void Registry::unloadWatchlet(QPluginLoader *loader)
{
	QString file = loader->fileName();
	WatchletPluginInterface *plugin = qobject_cast<WatchletPluginInterface*>(loader->instance());
	QStringList watchlets = plugin->watchlets();

	foreach (const QString& watchlet, watchlets) {
		emit watchletUnloaded(watchlet);
		_watchletIds.remove(watchlet);
	}

	_watchlets.removeAll(plugin);
	_watchletFiles.remove(file);
	_watcher->removePath(file);

	qDebug() << "Now unloading" << file;
	if (!loader->unload()) {
		qWarning() << "Could not unload plugin" << file;
	}

	delete loader;
}

void Registry::handlePluginDirectoryChanged(const QString &path)
{
	// If the directory changed, rescan it to discover new plugins.
	if (path == SOWATCH_DRIVERS_DIR) {
		QDir dir(path);
		foreach (QString entry, dir.entryList(QDir::Files)) {
			QString file = dir.absoluteFilePath(entry);
			if (!_driverFiles.contains(file)) {
				loadDriver(file);
			}
		}
	} else if (path == SOWATCH_NOTIFICATIONS_DIR) {
		QDir dir(path);
		foreach (QString entry, dir.entryList(QDir::Files)) {
			QString file = dir.absoluteFilePath(entry);
			if (!_providerFiles.contains(file)) {
				loadNotificationProvider(file);
			}
		}
	} else if (path == SOWATCH_WATCHLETS_DIR) {
		QDir dir(path);
		foreach (QString entry, dir.entryList(QDir::Files)) {
			QString file = dir.absoluteFilePath(entry);
			if (!_watchletFiles.contains(file)) {
				loadWatchlet(file);
			}
		}
	}
}

void Registry::handlePluginFileChanged(const QString &file)
{
	if (_driverFiles.contains(file)) {
		unloadDriver(_driverFiles[file]);
	}
	if (_providerFiles.contains(file)) {
		unloadNotificationProvider(_providerFiles[file]);
	}
	if (_watchletFiles.contains(file)) {
		unloadWatchlet(_watchletFiles[file]);
	}
}
