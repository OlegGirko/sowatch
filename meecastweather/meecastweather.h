#ifndef MEECASTWEATHER_H
#define MEECASTWEATHER_H

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <sowatch.h>

namespace sowatch
{

class MeeCastWeather : public WeatherNotification
{
    Q_OBJECT

public:
	explicit MeeCastWeather(QObject *parent = 0);

	static QString configFilePath();

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

#endif // MEECASTWEATHER_H
