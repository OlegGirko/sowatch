#ifndef SOWATCH_WEATHERNOTIFICATION_H
#define SOWATCH_WEATHERNOTIFICATION_H

#include "notification.h"

namespace sowatch
{

class WeatherNotification : public Notification
{
    Q_OBJECT
public:
    explicit WeatherNotification(QObject *parent = 0);

	enum WeatherType {
		UnknownWeather = 0,
		Sunny,
		Cloudy,
		Rain,
		Snow,
		Thunderstorm
	};

	enum Unit {
		UnknownUnit = 0,
		Celsius,
		Fahrenheit
	};

	virtual WeatherType forecast() = 0;
	virtual int temperature() = 0;
	virtual Unit temperatureUnits() = 0;
};

}

#endif // WEATHERNOTIFICATION_H
