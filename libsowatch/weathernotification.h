#ifndef SOWATCH_WEATHERNOTIFICATION_H
#define SOWATCH_WEATHERNOTIFICATION_H

#include "notification.h"

namespace sowatch
{

class WeatherNotification : public Notification
{
    Q_OBJECT
	Q_ENUMS(WeatherType Unit)
	Q_PROPERTY(WeatherType forecast READ forecast NOTIFY forecastChanged)
	Q_PROPERTY(int temperature READ temperature NOTIFY temperatureChanged)
	Q_PROPERTY(Unit temperatureUnits READ temperatureUnits NOTIFY temperatureUnitsChanged)

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

	Priority priority() const;

	virtual WeatherType forecast() const = 0;
	virtual int temperature() const = 0;
	virtual Unit temperatureUnits() const = 0;

signals:
	void forecastChanged();
	void temperatureChanged();
	void temperatureUnitsChanged();

protected:
	/** Useful helper functions. */
	static qreal convertTemperature(qreal temp, Unit from, Unit to);
	static int convertTemperature(int temp, Unit from, Unit to);
};

}

QML_DECLARE_TYPE(sowatch::WeatherNotification)
QML_DECLARE_TYPE(sowatch::WeatherNotification::WeatherType)
QML_DECLARE_TYPE(sowatch::WeatherNotification::Unit)

#endif // WEATHERNOTIFICATION_H
