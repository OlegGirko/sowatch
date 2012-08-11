#include "testnotification.h"
#include "testnotificationprovider.h"

using namespace sowatch;

TestNotificationProvider::TestNotificationProvider(QObject *parent) :
    NotificationProvider(parent),
    _timer(new QTimer(this))
{
	QTimer::singleShot(15000, this, SLOT(generateInitialNotification()));
	connect(_timer, SIGNAL(timeout()), SLOT(generateNotification()));
	_timer->setInterval(60000);
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
	                                           "A friend",
	                                           "I will keep talking to you");
	emit incomingNotification(n);
}
