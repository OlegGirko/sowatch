#include <QtCore/QtDebug>
#include <QtCore/QTimer>
#include "harmaccuweather.h"
#include "harmaccuprovider.h"

using namespace sowatch;

HarmAccuProvider::HarmAccuProvider(int updateTime, QObject *parent) :
	NotificationProvider(parent),
	_updateTime(updateTime)
{
	// Give some time to send the notification
	QTimer::singleShot(2000, this, SLOT(generateNotification()));
}

HarmAccuProvider::~HarmAccuProvider()
{

}

void HarmAccuProvider::generateNotification()
{
	QSettings* s = HarmAccuWeather::getAccuweatherData();
	if (s->contains("LastUpdate")) {
		qDebug() << "generate harmaccuweather notification";
		emit incomingNotification(new HarmAccuWeather(_updateTime, this));
	}
	delete s;
}
