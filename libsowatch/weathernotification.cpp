#include "weathernotification.h"

using namespace sowatch;

WeatherNotification::WeatherNotification(QObject *parent) :
    Notification(parent)
{
}

Notification::Priority WeatherNotification::priority() const
{
	return Silent;
}

qreal WeatherNotification::convertTemperature(qreal temp, Unit from, Unit to)
{
	if (from == to) {
		return temp;
	} else if (from == Celsius && to == Fahrenheit) {
		return temp * (9.0f/5.0f) + 32.0f;
	} else if (from == Fahrenheit && to == Celsius) {
		return (temp - 32.0f) * (5.0f/9.0f);
	} else {
		return 0.0f;
	}
}


int WeatherNotification::convertTemperature(int temp, Unit from, Unit to)
{
	if (from == to) {
		return temp;
	} else {
		return qRound(convertTemperature(static_cast<qreal>(temp), from, to));
	}
}
