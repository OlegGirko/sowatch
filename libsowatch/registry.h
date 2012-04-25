#ifndef SOWATCH_REGISTRY_H
#define SOWATCH_REGISTRY_H

#include <QtCore/QPluginLoader>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QFileSystemWatcher>
#include "sowatch_global.h"

namespace sowatch
{

class WatchPluginInterface;
class NotificationPluginInterface;
class WatchletPluginInterface;

class SOWATCH_EXPORT Registry : public QObject
{
	Q_OBJECT

public:
	static Registry* registry();

	inline QList<WatchPluginInterface*> getWatchPlugins() {
		return _drivers;
	}

	inline QList<NotificationPluginInterface*> getNotificationPlugins() {
		return _providers;
	}

	inline QList<WatchletPluginInterface*> getWatchletPlugins() {
		return _watchlets;
	}

	inline WatchPluginInterface* getWatchPlugin(const QString& id) {
		return _driverIds.value(id, 0);
	}

	inline NotificationPluginInterface* getNotificationPlugin(const QString& id) {
		return _providerIds.value(id, 0);
	}

	inline WatchletPluginInterface* getWatchletPlugin(const QString& id) {
		return _watchletIds.value(id, 0);
	}

protected:
	Registry();
	~Registry();

private:
	static Registry* singleRegistry;

	QFileSystemWatcher* _watcher;

	QList<WatchPluginInterface*> _drivers;
	QList<NotificationPluginInterface*> _providers;
	QList<WatchletPluginInterface*> _watchlets;

	QMap<QString, QPluginLoader*> _driverFiles;
	QMap<QString, QPluginLoader*> _providerFiles;
	QMap<QString, QPluginLoader*> _watchletFiles;

	QMap<QString, WatchPluginInterface*> _driverIds;
	QMap<QString, NotificationPluginInterface*> _providerIds;
	QMap<QString, WatchletPluginInterface*> _watchletIds;

	void loadDrivers();
	void loadNotificationProviders();
	void loadWatchlets();

	void loadDriver(const QString& file);
	void loadNotificationProvider(const QString& file);
	void loadWatchlet(const QString& file);

	void unloadDriver(QPluginLoader* loader);
	void unloadNotificationProvider(QPluginLoader* loader);
	void unloadWatchlet(QPluginLoader* loader);

private slots:
	void handlePluginDirectoryChanged(const QString& path);
	void handlePluginFileChanged(const QString& file);
};

}

#endif // SOWATCH_REGISTRY_H
