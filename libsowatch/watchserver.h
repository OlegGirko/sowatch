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
class WatchletsModel;
class NotificationProvider;
class NotificationsModel;

class SOWATCH_EXPORT WatchServer : public QObject
{
    Q_OBJECT
	Q_PROPERTY(Watch* watch READ watch CONSTANT)
	Q_PROPERTY(Watchlet* idleWatchlet READ idleWatchlet WRITE setIdleWatchlet)
	Q_PROPERTY(Watchlet* notificationWatchlet READ notificationWatchlet WRITE setNotificationWatchlet)

public:
	explicit WatchServer(Watch *watch, QObject *parent = 0);

	Watch* watch();
	const Watch* watch() const;

	Watchlet *idleWatchlet();
	void setIdleWatchlet(Watchlet *watchlet);

	Watchlet *notificationWatchlet();
	void setNotificationWatchlet(Watchlet *watchlet);

	const WatchletsModel * watchlets() const;

	void addWatchlet(Watchlet *watchlet);
	void insertWatchlet(int position, Watchlet *watchlet);
	void moveWatchlet(const Watchlet *watchlet, int to);
	void removeWatchlet(const Watchlet *watchlet);

	void addProvider(NotificationProvider *provider);
	void removeProvider(const NotificationProvider *provider);

	/** Get a list of all current live notifications. */
	const NotificationsModel * notifications() const;

public slots:
	void postNotification(Notification *notification);
	void nextNotification();

	/** Run the selected watchlet (set it as active, but not as current). */
	void runWatchlet(Watchlet* watchlet);
	/** Run the selected watchlet and set it as the current.
	    If the watch is disconnected, a notification arrives, etc. */
	void openWatchlet(Watchlet* watchlet);
	void openWatchlet(const QString& id);
	/** Close the active watchlet (and return back to the current watc) */
	void closeWatchlet();
	/** Close the active watchlet, advance the current watchlet carousel and open it */
	void nextWatchlet();

	void syncTime();

signals:
	void watchConnected();
	void watchDisconnected();

private:
	Watch* _watch;

	/** The amount of seconds that have to pass for a notification to be considered "outdated" and not shown. */
	int _oldNotificationThreshold;

	/** The watchlet that is activated whenever the watch goes to the idle screen (optional). */
	Watchlet *_idleWatchlet;
	/** The watchlet that is used to display notifications (optional). */
	Watchlet *_notificationWatchlet;

	/** A list of watchlets, in order. */
	WatchletsModel *_watchlets;
	/** Stores all the watchlets with a given watchled id. */
	QMap<QString, Watchlet*> _watchletIds;

	/** Stores current live notifications. */
	NotificationsModel *_notifications;
	/** A list of notifications that are yet to be shown to the user. */
	QQueue<Notification*> _pendingNotifications;
	/** Stores the count of notifications hidden between each notification object. */
	QMap<Notification*, uint> _notificationCounts;

	/** Active watchlet is the one that has "focus" right now. */
	Watchlet* _activeWatchlet;
	/** Current watchlet is the app watchlet (not idle, not notification) that is current in the carrousel. */
	Watchlet* _currentWatchlet;
	/** The current watchlet index if any, for use by nextWatchlet() */
	int _currentWatchletIndex;

	/** Used for periodic watch time syncing. */
	QTimer* _syncTimeTimer;

	/** Remove a notification of a certain type. */
	void removeNotification(Notification::Type type, Notification* n);

	void setWatchletProperties(Watchlet *watchlet);
	void unsetWatchletProperties(Watchlet *watchlet);
	void activateWatchlet(Watchlet *watchlet);
	void deactivateActiveWatchlet();
	void activateCurrentWatchlet();
	void goToIdle();

private slots:
	void handleWatchConnected();
	void handleWatchDisconnected();
	void handleWatchIdling();
	void handleNextWatchletRequested();
	void handleWatchletRequested(const QString& id);
	void handleCloseWatchletRequested();

	void handleNotificationChanged();
	void handleNotificationDismissed();
	void handleNotificationDestroyed();
};

}

#endif // SOWATCH_WATCHSERVER_H
