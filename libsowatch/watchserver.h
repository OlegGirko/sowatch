#ifndef SOWATCH_WATCHSERVER_H
#define SOWATCH_WATCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSignalMapper>

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

public:
	explicit WatchServer(Watch* watch, QObject* parent = 0);

	Watch* watch();

	void addProvider(NotificationProvider* provider);

	void runWatchlet(const QString& id);
	void closeWatchlet();

protected:
	Watch* _watch;
	Watchlet* _currentWatchlet;

	QMap<QString, Watchlet*> _watchlets;
	QList<NotificationProvider*> _providers;

	void registerWatchlet(Watchlet *watchlet);

protected slots:
	void watchConnected();
	void watchDisconnected();
	void notificationEmitted(const Notification& notification);
	void unreadCountUpdated(Notification::Type type);
	void incomingCall(const QString& displayText);
	void endIncomingCall();

friend class Watchlet;
};

}

#endif // SOWATCH_WATCHSERVER_H
