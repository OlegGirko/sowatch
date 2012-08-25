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

	const static QLatin1String configFilePath;

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

private:
	int convertTemperatureToUserUnit(int temp, Unit unit);

private slots:
	void fileChanged(const QString& path);
	void parseConfigFile();
	void parseStationFile();
	void handleTimeout();

private:
	QFileSystemWatcher* _watcher;
	QTimer* _timer;

	QString _stationFilePath;

	bool _configFileChanged;
	bool _stationFileChanged;

	Unit _tempUnit;
	QString _location;

	QDateTime _lastUpdate;
	int _lastTemp;
	int _lastCode;
	QString _lastText;

};

}

#endif // MEECASTWEATHER_H
