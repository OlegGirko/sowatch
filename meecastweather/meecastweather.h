#ifndef HARMACCUWEATHER_H
#define HARMACCUWEATHER_H

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <sowatch.h>

namespace sowatch
{

class HarmAccuWeather : public WeatherNotification
{
    Q_OBJECT

public:
	explicit HarmAccuWeather(QObject *parent = 0);

	static QSettings* getAccuweatherData();

	Type type() const;
	uint count() const;
	QDateTime dateTime() const;
	QString title() const;
	QString body() const;

	WeatherType forecast();
	int temperature();
	Unit temperatureUnits();

	void activate();
	void dismiss();

private slots:
	void fileChanged(const QString& path);
	void update();

private:
	QFileSystemWatcher* _watcher;
	QTimer* _timer;

	bool _metric;
	QDateTime _lastUpdate;
	QString _lastLocation;
	int _lastTemp;
	int _lastWxCode;

};

}

#endif // HARMACCUWEATHER_H
