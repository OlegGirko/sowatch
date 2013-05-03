#ifndef SOWATCH_WATCH_H
#define SOWATCH_WATCH_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtGui/QPaintDevice>
#include <QtGui/QImage>
#include "notification.h"
#include "weathernotification.h"
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT Watch : public QObject, public QPaintDevice
{
	Q_OBJECT
	Q_PROPERTY(QString model READ model CONSTANT)
	Q_PROPERTY(bool connected READ isConnected)
	Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
	Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
	Q_PROPERTY(bool charging READ charging NOTIFY chargingChanged)

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

	/** Sets the current date/time on the watch. */
	virtual void setDateTime(const QDateTime& dateTime) = 0;
	/** Asynchronously queries battery date/time from the watch; once the
	 *  query is finished, dateTimeChanged() will be signaled and dateTime()
	 *  will return the updated value.
	 *  This is not mandatory and returning currentDateTime() is just fine.
     */
	virtual void queryDateTime() = 0;
	/** Gets the current date/time as last fetched from the watch. */
	virtual QDateTime dateTime() const = 0;

	/** Asynchronously queries battery level from the watch. */
	virtual void queryBatteryLevel() = 0;
	/** Gets the battery level (range [0-100]) as last read from the watch. */
	virtual int batteryLevel() const = 0;

	/** Asynchronously queries whether the watch is connected to a charger. */
	virtual void queryCharging() = 0;
	virtual bool charging() const = 0;

	/** Tells the watch to update the unread notifications count, if visible. */
	virtual void updateNotificationCount(Notification::Type type, int count) = 0;

	/** Tells the watch to update the current weather forecast, if visible. */
	virtual void updateWeather(WeatherNotification* weather) = 0;

public slots:
	/** Go back to the idle screen. */
	virtual void displayIdleScreen() = 0;
	/** A standard notification; it's up to the watch when to stop showing it. */
	virtual void displayNotification(Notification* notification) = 0;
	/** Enter application mode; after this, server can draw on the QPaintDevice. */
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
	/** Emitted once the queryDateTime() operation is completed. */
	void dateTimeChanged();
	/** Emitted once the queryBatteryLevel() operation is completed. */
	void batteryLevelChanged();
	/** Emitted once the queryCharging() operation is completed. */
	void chargingChanged();
	/** A button has been pressed. */
	void buttonPressed(int button);
	/** A button has been pressed and then released. */
	void buttonReleased(int button);
};

}

#endif // SOWATCH_WATCH_H
