#include <QtCore/QDebug>
#include <sowatch.h>
#include "daemon.h"

using namespace sowatch;

Daemon::Daemon(QObject *parent) :
	QObject(parent),
	_registry(Registry::registry()),
	_settings(new GConfKey("/apps/sowatch", this))
{
	connect(_settings, SIGNAL(subkeyChanged(QString)), SLOT(settingsChanged(QString)));
	QStringList activeWatches = _settings->value("active-watches").toStringList();
	foreach (const QString& s, activeWatches) {
		startWatch(s);
	}
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
	QScopedPointer<ConfigKey> watchSettings(_settings->getSubkey(name));

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
}

#if TODO
void Daemon::initWatch(Watch* watch, QSettings& settings)
{
	int size;


}
#endif

void Daemon::settingsChanged(const QString &subkey)
{
	qDebug() << "Daemon settings changed" << subkey;
	if (subkey == "active-watches") {
		QSet<QString> confActiveWatches = _settings->value("active-watches").toStringList().toSet();
		QSet<QString> curActiveWatches = _servers.keys().toSet();
		QSet<QString> removed = curActiveWatches - confActiveWatches;
		QSet<QString> added = confActiveWatches - curActiveWatches;
		foreach (const QString& s, removed) {
			stopWatch(s);
		}
		foreach (const QString& s, added) {
			startWatch(s);
		}
	}
}
