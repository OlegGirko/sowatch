#ifndef SOWATCH_WATCHSERVER_H
#define SOWATCH_WATCHSERVER_H

#include <QtCore/QList>
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

	void runWatchlet(const QString& id);
	void closeWatchlet();

protected:
	Watch* _watch;

	char _nextWatchletButton;

	QMap<QString, Watchlet*> _watchlets;

	/** Stores current notifications, classified by type. */
	QList<Notification*> _notifications[Notification::TypeCount];
	QQueue<Notification*> _pendingNotifications;

	Watchlet* _currentWatchlet;
	char _currentWatchletIndex;

	void registerWatchlet(Watchlet *watchlet);
	void reactivateCurrentWatchlet();
	void nextWatchlet();

	void nextNotification();
	uint getNotificationCount(Notification::Type type);

	void goToIdle();

protected slots:
	void watchConnected();
	void watchDisconnected();
	void watchIdling();
	void watchButtonPress(int button);
	void notificationReceived(Notification* notification);
	void notificationChanged();
	void notificationCleared();

friend class Watchlet;
};

}

#endif // SOWATCH_WATCHSERVER_H
