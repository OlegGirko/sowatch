#include "harmaccuweather.h"

using namespace sowatch;

HarmAccuWeather::HarmAccuWeather(int updateTime, QObject *parent) :
	WeatherNotification(parent),
	_updateTimer(new QTimer(this)),
	_lastUpdate(QDateTime::fromTime_t(0))
{
	connect(_updateTimer, SIGNAL(timeout()), SLOT(update()));
	_updateTimer->setInterval(updateTime * 1000);
	_updateTimer->start();
	update();
}

QSettings* HarmAccuWeather::getAccuweatherData()
{
	return new QSettings("AccuWeather, Inc.", "awxapp");
}

Notification::Type HarmAccuWeather::type() const
{
	return Notification::WeatherNotification;
}

uint HarmAccuWeather::count() const
{
	return 1;
}

QDateTime HarmAccuWeather::dateTime() const
{
	return _lastUpdate;
}

QString HarmAccuWeather::title() const
{
	return _lastLocation;
}

QString HarmAccuWeather::body() const
{
	switch (_lastWxCode) {
	case 33:
		return tr("Clear");
	case 6:
	case 38:
		return tr("Mostly cloudy");
	case 35:
		return tr("Part. cloudy");
	case 7:
		return tr("Cloudy");
	default:
		return QString("%1").arg(_lastWxCode);
	}
}

WeatherNotification::WeatherType HarmAccuWeather::forecast()
{
	switch (_lastWxCode) {
	case 33:
		return Sunny;
	case 6:
	case 7:
	case 35:
	case 38:
		return Cloudy;
	default:
		return UnknownWeather;
	}
}

int HarmAccuWeather::temperature()
{
	return _lastTemp;
}

WeatherNotification::Unit HarmAccuWeather::temperatureUnits()
{
	return _metric ? Celsius : Fahrenheit;
}

void HarmAccuWeather::activate()
{
	// Launch accuweather?
}

void HarmAccuWeather::dismiss()
{
	// Do nothing
}

void HarmAccuWeather::update()
{
	QSettings* s = getAccuweatherData();

	QDateTime lastUpdate = s->value("LastUpdate").toDateTime();
	if (lastUpdate > _lastUpdate) {
		qDebug() << "updated weather info";
		_lastUpdate = lastUpdate;
		_metric = s->value("useMetric").toBool();
		_lastTemp = s->value("LastTemp").toInt();
		_lastLocation = s->value("LastLocation").toString();
		_lastWxCode = s->value("LastWxCode").toInt();
		emit changed();
	}

	delete s;
}
