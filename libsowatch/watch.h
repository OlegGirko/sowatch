#ifndef SOWATCH_WATCH_H
#define SOWATCH_WATCH_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtGui/QPaintDevice>
#include <QtGui/QImage>
#include "notification.h"

namespace sowatch
{

class Watch : public QObject, public QPaintDevice
{
	Q_OBJECT
	Q_PROPERTY(QString model READ model)
	Q_PROPERTY(bool connected READ isConnected)
	Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)

public:
	explicit Watch(QObject* parent = 0);
	~Watch();

	/** Return a string identiyfying this watch's model. */
	virtual QString model() const = 0;
	/** Should return true if the watch is connected. */
	virtual bool isConnected() const = 0;

	/** Indicates if watch is too busy atm and we should limit frame rate. */
	virtual bool busy() const = 0;

	/** Changes the current date/time on the watch. */
	virtual QDateTime dateTime() = 0;
	virtual void setDateTime(const QDateTime& dateTime) = 0;

	/** Tells the watch to update the unread notifications count, if visible. */
	virtual void updateNotificationCount(Notification::Type type, int count) = 0;

signals:
	/** The watch has been found and linked to. */
	void connected();
	/** The watch connection has been lost. */
	void disconnected();
	/** The watch has returned to the idle screen by either inactivity or notification cleared/timeout. */
	void idling();
	void buttonPressed(int button);
	void buttonReleased(int button);

public slots:
	/** Go back to the idle screen. */
	virtual void displayIdleScreen() = 0;
	/** A standard notification; it's up to the watch when to stop showing it. */
	virtual void displayNotification(Notification* n) = 0;
	/** Enter application mode. */
	virtual void displayApplication() = 0;
};

}

#endif // SOWATCH_WATCH_H
