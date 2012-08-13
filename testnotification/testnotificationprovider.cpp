#include "testnotification.h"
#include "testnotificationprovider.h"

using namespace sowatch;

int TestNotificationProvider::_counter = 1;

TestNotificationProvider::TestNotificationProvider(QObject *parent) :
    NotificationProvider(parent),
    _timer(new QTimer(this))
{
	QTimer::singleShot(1000, this, SLOT(generateInitialNotification()));
	QTimer::singleShot(1200, this, SLOT(generateNotification()));
	QTimer::singleShot(1400, this, SLOT(generateNotification()));
	QTimer::singleShot(1600, this, SLOT(generateNotification()));
	QTimer::singleShot(1800, this, SLOT(generateNotification()));
	QTimer::singleShot(2000, this, SLOT(generateNotification()));
	QTimer::singleShot(2200, this, SLOT(generateNotification()));
	QTimer::singleShot(2400, this, SLOT(generateInitialNotification()));
	connect(_timer, SIGNAL(timeout()), SLOT(generateNotification()));
	_timer->setInterval(60000);
	//_timer->start();
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
