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

	virtual QString model() const = 0;
	virtual bool isConnected() const = 0;

	/** Indicates if watch is too busy atm and we should limit frame rate. */
	virtual bool busy() const = 0;

	virtual QDateTime dateTime() = 0;
	virtual void setDateTime(const QDateTime& dateTime) = 0;

	virtual void updateNotificationCount(Notification::Type type, int count) = 0;

signals:
	void connected();
	void disconnected();
	void buttonPressed(int button);
	void buttonReleased(int button);

public slots:
	virtual void vibrate(bool on) = 0;
	virtual void showNotification(const Notification& n) = 0;
};

}

#endif // SOWATCH_WATCH_H
