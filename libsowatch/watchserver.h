#ifndef SOWATCH_WATCHSERVER_H
#define SOWATCH_WATCHSERVER_H

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <QtCore/QTimer>

#include "sowatch_global.h"
#include "notification.h"

namespace sowatch
{

class Watch;
class Watchlet;
class NotificationProvider;
class WeatherNotification;

class SOWATCH_EXPORT WatchServer : public QObject
{
    Q_OBJECT
	Q_PROPERTY(Watch* watch READ watch CONSTANT)
	Q_PROPERTY(QString nextWatchletButton READ nextWatchletButton WRITE setNextWatchletButton)

public:
	explicit WatchServer(Watch *watch, QObject *parent = 0);

	Watch* watch();

	QString nextWatchletButton() const;
	void setNextWatchletButton(const QString& value);

	void addWatchlet(Watchlet *watchlet);
	void insertWatchlet(int position, Watchlet *watchlet);
	void removeWatchlet(const Watchlet *watchlet);

	void addProvider(NotificationProvider *provider);
	void removeProvider(const NotificationProvider *provider);

	/** Get a list of all current live notifications. */
	QList<Notification*> liveNotifications();

public slots:
	void postNotification(Notification *notification);
	void nextNotification();

	void runWatchlet(Watchlet* watchlet);
	void runWatchlet(const QString& id);
	void closeWatchlet();
	void nextWatchlet();

	void syncTime();

signals:
	void watchConnected();
	void watchDisconnected();

private:
	Watch* _watch;

	/** The watch button that causes next watchlet to be run. */
	int _nextWatchletButton;
	/** The amount of seconds that have to pass for a notification to be considered "outdated" and not shown. */
	int _oldNotificationThreshold;

	/** A list of watchlets, in order. */
	QList<Watchlet*> _watchlets;
	/** Stores all the watchlets with a given watchled id. */
	QMap<QString, Watchlet*> _watchletIds;

	/** Stores current live notifications, classified by type. */
	QList<Notification*> _notifications[Notification::TypeCount];
	/** A list of notifications that are yet to be shown to the user. */
	QQueue<Notification*> _pendingNotifications;
	/** Stores the count of notifications hidden between each notification object. */
	QMap<Notification*, uint> _notificationCounts;
	/** We store a currently live weather forecast. */
	WeatherNotification* _weather;

	/** Current watchlet. */
	Watchlet* _currentWatchlet;
	/** Is the current watchlet active? */
	bool _currentWatchletActive;
	/** The current watchlet index if any, for use by nextWatchlet() */
	int _currentWatchletIndex;

	/** Used for periodic watch time syncing. */
	QTimer* _syncTimeTimer;

	/** Counts all notifications from a given type. */
	uint getNotificationCount(Notification::Type type);

	void deactivateCurrentWatchlet();
	void reactivateCurrentWatchlet();
	void goToIdle();

private slots:
	void handleWatchConnected();
	void handleWatchDisconnected();
	void handleWatchIdling();
	void handleWatchButtonPress(int button);

	void handleNotificationChanged();
	void handleNotificationDismissed();
	void handleNotificationDestroyed();
};

}

#endif // SOWATCH_WATCHSERVER_H
