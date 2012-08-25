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
		PartlyCloudy,
		Cloudy,
		Fog,
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

	/** Quite useful helper function. */
	static qreal convertTemperature(qreal temp, Unit from, Unit to);
	static int convertTemperature(int temp, Unit from, Unit to);
};



}

#endif // WEATHERNOTIFICATION_H
