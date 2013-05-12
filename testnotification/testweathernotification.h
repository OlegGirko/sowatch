#ifndef TESTWEATHERNOTIFICATION_H
#define TESTWEATHERNOTIFICATION_H

#include <sowatch.h>

namespace sowatch
{

class TestWeatherNotification : public WeatherNotification
{
    Q_OBJECT

public:
	explicit TestWeatherNotification(QObject *parent = 0);

	Type type() const;
	uint count() const;
	QDateTime dateTime() const;
	QString title() const;
	QString body() const;

	WeatherType forecast() const;
	int temperature() const;
	Unit temperatureUnits() const;

	void activate();
	void dismiss();
};

}

QML_DECLARE_TYPE(sowatch::TestWeatherNotification)

#endif // TESTWEATHERNOTIFICATION_H
