#include "testweathernotification.h"

using namespace sowatch;

TestWeatherNotification::TestWeatherNotification(QObject *parent)
    : WeatherNotification(parent)
{
}


Notification::Type TestWeatherNotification::type() const
{
	return Notification::WeatherNotification;
}

uint TestWeatherNotification::count() const
{
	return 1;
}

QDateTime TestWeatherNotification::dateTime() const
{
	return QDateTime::currentDateTime();
}

QString TestWeatherNotification::title() const
{
	return "My city";
}

QString TestWeatherNotification::body() const
{
	return "Mostly sunny";
}

WeatherNotification::WeatherType TestWeatherNotification::forecast() const
{
	return Sunny;
}

int TestWeatherNotification::temperature() const
{
	return 30;
}

WeatherNotification::Unit TestWeatherNotification::temperatureUnits() const
{
	return Celsius;
}

void TestWeatherNotification::activate()
{
	// Do nothing
}

void TestWeatherNotification::dismiss()
{
	deleteLater(); // We do not want to keep those around.
}
