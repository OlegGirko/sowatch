#ifndef MEEGOHANDSETNOTIFICATION_H
#define MEEGOHANDSETNOTIFICATION_H

#include <sowatch.h>
#include <notificationsystem/notification.h>
#include <notificationsystem/notificationparameters.h>

class WatchNotificationSink;

class MeegoHandsetNotification : public sowatch::Notification
{
    Q_OBJECT
public:
	explicit MeegoHandsetNotification(const ::Notification& n, QObject *parent = 0);

	Type type() const;
	uint count() const;
	QDateTime dateTime() const;
	QString title() const;
	QString body() const;
	void activate();
	void clear();

signals:
	void changed();
	void cleared();

protected:
	void changeTo(const ::Notification& notification);

protected:
	::Notification _n;

friend class WatchNotificationSink;
};

#endif // MEEGOHANDSETNOTIFICATION_H
