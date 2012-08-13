#ifndef TESTNOTIFICATIONPROVIDER_H
#define TESTNOTIFICATIONPROVIDER_H

#include <sowatch.h>

namespace sowatch
{

class TestNotification;

class TestNotificationProvider : public NotificationProvider
{
    Q_OBJECT

public:
	explicit TestNotificationProvider(QObject *parent = 0);
	~TestNotificationProvider();

private slots:
	void generateInitialNotification();
	void generateNotification();

private:
	static int _counter;
	QTimer *_timer;
};

}

#endif // TESTNOTIFICATIONPROVIDER_H
