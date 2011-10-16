#ifndef SOWATCH_REGISTRY_H
#define SOWATCH_REGISTRY_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include "sowatch_global.h"

namespace sowatch
{

class WatchPluginInterface;
class NotificationPluginInterface;
class WatchletPluginInterface;

class SOWATCH_EXPORT Registry
{
public:
	static Registry* registry();

	inline WatchPluginInterface* getWatchPlugin(const QString& id) {
		return _drivers.value(id, 0);
	}

	inline NotificationPluginInterface* getNotificationPlugin(const QString& id) {
		return _providers.value(id, 0);
	}

	inline WatchletPluginInterface* getWatchletPlugin(const QString& id) {
		return _watchlets.value(id, 0);
	}

protected:
	Registry();
	~Registry();

private:
	static Registry* singleRegistry;

	QMap<QString, WatchPluginInterface*> _drivers;
	QMap<QString, NotificationPluginInterface*> _providers;
	QMap<QString, WatchletPluginInterface*> _watchlets;

	void loadDrivers();
	void loadNotificationProviders();
	void loadWatchlets();
};

}

#endif // SOWATCH_REGISTRY_H
