#ifndef SOWATCH_NOTIFICATION_H
#define SOWATCH_NOTIFICATION_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT Notification
{
public:
	enum Type {
		OtherNotification = 0,
		CallNotification,
		EmailNotification,
		MissedCallNotification,
		SmsNotification,
		ImNotification,
		CalendarNotification,
		TypeCount
	};

	Notification(Type type, const QDateTime& dateTime, QString title, QString body);
	~Notification();

	inline Type type() const { return _type; }
	inline QDateTime dateTime() const { return _dateTime; }
	inline QString title() const { return _title; }
	inline QString body() const { return _body; }

protected:
	Type _type;
	QDateTime _dateTime;
	QString _title;
	QString _body;
};

}

#endif // SOWATCH_NOTIFICATION_H
