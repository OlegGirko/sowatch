#ifndef SOWATCH_NOTIFICATION_H
#define SOWATCH_NOTIFICATION_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QImage>
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT Notification : public QObject
{
	Q_OBJECT
	Q_ENUMS(Type)

public:
	enum Type {
		OtherNotification = 0,
		CallNotification,
		MissedCallNotification,
		SmsNotification,
		MmsNotification,
		ImNotification,
		EmailNotification,
		CalendarNotification,
		TypeCount
	};

	explicit Notification(QObject *parent = 0);
	virtual ~Notification();

	virtual Type type() const = 0;
	virtual uint count() const = 0;
	virtual QDateTime dateTime() const = 0;
	virtual QString displayTime() const;
	virtual QString title() const = 0;
	virtual QString body() const = 0;
	virtual QImage image() const;

public slots:
	virtual void activate() = 0;
	virtual void clear() = 0;

signals:
	void changed();
	void cleared();
};

}

#endif // SOWATCH_NOTIFICATION_H
