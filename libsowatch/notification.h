#ifndef SOWATCH_NOTIFICATION_H
#define SOWATCH_NOTIFICATION_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QImage>
#include <QtDeclarative/QtDeclarative>
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT Notification : public QObject
{
	Q_OBJECT
	Q_ENUMS(Type)
	Q_PROPERTY(Type type READ type CONSTANT)
	Q_PROPERTY(uint count READ count NOTIFY countChanged)
	Q_PROPERTY(QDateTime dateTime READ dateTime NOTIFY dateTimeChanged)
	Q_PROPERTY(QString displayTime READ displayTime NOTIFY displayTimeChanged STORED false)
	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString body READ body NOTIFY bodyChanged)
	Q_PROPERTY(QImage image READ image NOTIFY imageChanged)

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
	/** Do something on this notification; open the application that caused it, answer, etc. */
	virtual void activate() = 0;
	/** Dismiss this notification. */
	virtual void dismiss() = 0;

signals:
	/* For the convenience of QML users */
	void countChanged();
	void dateTimeChanged();
	void displayTimeChanged();
	void titleChanged();
	void bodyChanged();
	void imageChanged();

	/** Generic "changed" signal if any of the properties changes; can be batched. */
	void changed();

	/** The notification has been dismissed by the user or via dismiss(). */
	void dismissed();
	/* Provider of this notification object should delete it after dismissal. */
};

}

QML_DECLARE_TYPE(sowatch::Notification)

#endif // SOWATCH_NOTIFICATION_H
