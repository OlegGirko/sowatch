#ifndef SOWATCH_WATCH_H
#define SOWATCH_WATCH_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtGui/QPaintDevice>
#include <QtGui/QImage>
#include "notification.h"

namespace sowatch
{

class Watch : public QObject, public QPaintDevice
{
	Q_OBJECT
	Q_PROPERTY(QString model READ model CONSTANT)
	Q_PROPERTY(bool connected READ isConnected)
	Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)

public:
	explicit Watch(QObject* parent = 0);
	~Watch();

	/** Return a string identifying this watch's model. */
	virtual QString model() const = 0;

	/** Names for all the buttons this watch has. Order is important. */
	virtual QStringList buttons() const = 0;

	/** Should return true if the watch is connected. */
	virtual bool isConnected() const = 0;
	/** Indicates if watch is too busy atm and we should limit frame rate. */
	virtual bool busy() const = 0;

	/** Gets the current date/time on the watch. */
	virtual QDateTime dateTime() = 0;
	/** Sets the current date/time on the watch. */
	virtual void setDateTime(const QDateTime& dateTime) = 0;

	/** Grabs a button from whatever is default function is for the current mode. */
	virtual void grabButton(int button) = 0;
	/** Restores a button to its default function. */
	virtual void ungrabButton(int button) = 0;

	/** Tells the watch to update the unread notifications count, if visible. */
	virtual void updateNotificationCount(Notification::Type type, int count) = 0;

public slots:
	/** Go back to the idle screen. */
	virtual void displayIdleScreen() = 0;
	/** A standard notification; it's up to the watch when to stop showing it. */
	virtual void displayNotification(Notification* notification) = 0;
	/** Enter application mode. */
	virtual void displayApplication() = 0;

	/** Vibrate for a while. The default implementation does nothing. */
	virtual void vibrate(int msecs);

signals:
	/** The watch has been found and linked to. */
	void connected();
	/** The watch connection has been lost. */
	void disconnected();
	/** The watch has returned to the idle screen by either inactivity or notification cleared/timeout. */
	void idling();
	/** A button has been pressed. */
	void buttonPressed(int button);
	/** A button has been pressed and then released. */
	void buttonReleased(int button);
};

}

#endif // SOWATCH_WATCH_H
