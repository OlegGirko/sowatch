#ifndef SOWATCH_WATCHSERVER_H
#define SOWATCH_WATCHSERVER_H

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QQueue>

#include "sowatch_global.h"
#include "notification.h"

namespace sowatch
{

class Watch;
class Watchlet;
class NotificationProvider;

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

public slots:
	void postNotification(Notification *notification);
	void runWatchlet(const QString& id);
	void closeWatchlet();
	void nextWatchlet();

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

	/** Current watchlet. */
	Watchlet* _currentWatchlet;
	/** The current watchlet index if any, for use by nextWatchlet() */
	int _currentWatchletIndex;

	void registerWatchlet(Watchlet *watchlet);
	void reactivateCurrentWatchlet();

	void nextNotification();
	uint getNotificationCount(Notification::Type type);

	void goToIdle();

private slots:
	void watchConnected();
	void watchDisconnected();
	void watchIdling();
	void watchButtonPress(int button);

	void notificationChanged();
	void notificationCleared();

friend class Watchlet;
};

}

#endif // SOWATCH_WATCHSERVER_H
