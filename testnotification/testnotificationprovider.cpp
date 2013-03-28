#include "testnotification.h"
#include "testnotificationprovider.h"

using namespace sowatch;

int TestNotificationProvider::_counter = 1;

TestNotificationProvider::TestNotificationProvider(QObject *parent) :
    NotificationProvider(parent),
    _timer(new QTimer(this))
{
	const int initial_delay = 2000;
	const int burst_num = 0;
	const int burst_delay = 500;
	const int extra_delay = 100 * 1000;
	QTimer::singleShot(initial_delay, this, SLOT(generateInitialNotification()));
	for (int i = 0; i < burst_num; i++) {
		QTimer::singleShot(initial_delay + burst_delay * (i+1), this, SLOT(generateNotification()));
	}
	connect(_timer, SIGNAL(timeout()), SLOT(generateNotification()));
	_timer->setInterval(extra_delay);
	_timer->start();
}

TestNotificationProvider::~TestNotificationProvider()
{
}

void TestNotificationProvider::generateInitialNotification()
{
	TestNotification *n = new TestNotification(Notification::EmailNotification,
	                                           "A friend",
	                                           "This is a test email notification");
	emit incomingNotification(n);
}

void TestNotificationProvider::generateNotification()
{
	TestNotification *n = new TestNotification(Notification::ImNotification,
	                                           QString("Friend %1").arg(_counter++),
	                                           "Lorem ipsum dolor sit amet. "
	                                           "Consectetur adipiscing elit. "
	                                           "Donec non congue augue.");
	emit incomingNotification(n);
}
