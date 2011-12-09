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
	explicit WatchServer(Watch* watch, QObject* parent = 0);

	Watch* watch();

	QString nextWatchletButton() const;
	void setNextWatchletButton(const QString& value);

	void addProvider(NotificationProvider* provider);

	/** Get a list of all current live notifications. */
	QList<Notification*> liveNotifications();

public slots:
	void postNotification(Notification *notification);
	void nextNotification();

	void runWatchlet(const QString& id);
	void closeWatchlet();
	void nextWatchlet();

	void syncTime();

private:
	Watch* _watch;

	/** The watch button that causes next watchlet to be run. */
	int _nextWatchletButton;
	/** The amount of seconds that have to pass for a notification to be considered "outdated" and not shown. */
	int _oldNotificationThreshold;

	/** A list of watchlets in order, for use by nextWatchlet() */
	QStringList _watchletIds;
	/** Actual Watchlet child objects by id. */
	QMap<QString, Watchlet*> _watchlets;

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
	/** The current watchlet index if any, for use by nextWatchlet() */
	int _currentWatchletIndex;

	/** Used for periodic time syncing. */
	QTimer* _syncTimeTimer;

	/** Called by Watchlet constructor to register itself as a child. */
	void registerWatchlet(Watchlet *watchlet);

	/** Counts all notifications from a given type. */
	uint getNotificationCount(Notification::Type type);

	void reactivateCurrentWatchlet();
	void goToIdle();

private slots:
	void watchConnected();
	void watchDisconnected();
	void watchIdling();
	void watchButtonPress(int button);

	void notificationChanged();
	void notificationDismissed();

friend class Watchlet;
};

}

#endif // SOWATCH_WATCHSERVER_H
