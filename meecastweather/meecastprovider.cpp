#include <QtCore/QtDebug>
#include <QtCore/QTimer>
#include "meecastweather.h"
#include "meecastprovider.h"

using namespace sowatch;

const QLatin1String MeeCastProvider::myId("meecast");

MeeCastProvider::MeeCastProvider(QObject *parent) :
	NotificationProvider(parent)
{
	// Give some time to send the first notification
	QTimer::singleShot(2000, this, SLOT(generateNotification()));
}

MeeCastProvider::~MeeCastProvider()
{
}

void MeeCastProvider::generateNotification()
{
	if (QFile::exists(MeeCastWeather::configFilePath)) {
		emit incomingNotification(new MeeCastWeather(this));
	}
}
