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

Registry::Registry()
{
	loadDrivers();
	loadNotificationProviders();
	loadWatchlets();
}

void Registry::loadDrivers()
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

void Registry::loadNotificationProviders()
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

	qDebug() << "loaded notification providers" << _providers.keys();
}

void Registry::loadWatchlets()
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
