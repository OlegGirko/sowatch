#include <QtCore/QtDebug>
#include <QtCore/QTimer>
#include "harmaccuweather.h"
#include "harmaccuprovider.h"

using namespace sowatch;

HarmAccuProvider::HarmAccuProvider(QObject *parent) :
	NotificationProvider(parent)
{
	// Give some time to send the first notification
	QTimer::singleShot(2000, this, SLOT(generateNotification()));
}

HarmAccuProvider::~HarmAccuProvider()
{
}

void HarmAccuProvider::generateNotification()
{
	QSettings* s = HarmAccuWeather::getAccuweatherData();
	if (s->contains("LastUpdate")) {
		qDebug() << "generating harmaccuweather notification";
		emit incomingNotification(new HarmAccuWeather(this));
	} else {
		qWarning() << "Accuweather config file does not seem to exist";
	}
	delete s;
}
