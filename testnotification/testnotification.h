#ifndef TESTNOTIFICATION_H
#define TESTNOTIFICATION_H

#include <sowatch.h>

namespace sowatch
{

class TestNotification : public Notification
{
	Q_OBJECT

public:
	explicit TestNotification(Type type, const QString& title, const QString& body, QObject *parent = 0);

	Type type() const;
	uint count() const;
	QDateTime dateTime() const;
	QString title() const;
	QString body() const;

	void activate();
	void dismiss();

private:
	Type _type;
	QDateTime _time;
	QString _title;
	QString _body;
};

}

QML_DECLARE_TYPE(sowatch::TestNotification)

#endif // TESTNOTIFICATION_H
