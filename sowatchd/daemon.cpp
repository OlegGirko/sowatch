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
	loadWatches();
	loadWatchlets();
}

void Daemon::loadDrivers()
{
	QDir dir(SOWATCH_DRIVERS_DIR);
	foreach (QString file, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(file));
		QObject *pluginObj = loader.instance();
		if (pluginObj) {
			WatchPluginInterface *plugin = qobject_cast<WatchPluginInterface*>(pluginObj);
			if (plugin) {
				QStringList drivers = plugin->drivers();
				foreach (const QString& driver, drivers) {
					_drivers[driver] = plugin;
				}
			}
		} else {
			qWarning() << "Invalid plugin" << file;
		}
	}

	qDebug() << "loaded drivers" << _drivers.keys();
}

void Daemon::loadWatches()
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
				_watches.append(watch);
			} else {
				qWarning() << "Driver" << driver << "refused to getWatch";
			}
		} else {
			qWarning() << "Invalid driver" << driver;
		}
	}

	settings.endArray();
	qDebug() << "handling" << _watches.size() << "watches";
}

void Daemon::loadWatchlets()
{
#if 0
	QDir dir(SOWATCH_WATCHLETS_DIR);
	foreach (QString file, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(file));
		QObject *pluginObj = loader.instance();
		if (pluginObj) {
			WatchPluginInterface *plugin = qobject_cast<WatchPluginInterface*>(pluginObj);
			if (plugin) {
				QStringList drivers = plugin->drivers();
				foreach (const QString& driver, drivers) {
					_drivers[driver] = plugin;
				}
			}
		}
	}
#endif
}
