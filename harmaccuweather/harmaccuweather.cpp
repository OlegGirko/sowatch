#include "harmaccuweather.h"

using namespace sowatch;

HarmAccuWeather::HarmAccuWeather(QObject *parent) :
	WeatherNotification(parent),
	_watcher(new QFileSystemWatcher(this)),
	_timer(new QTimer(this)),
	_lastUpdate(QDateTime::fromTime_t(0))
{
	// This only works on Harmattan either way, so I guess
	// hardcoding the path is OK.
	_watcher->addPath("/home/user/.config/AccuWeather, Inc./awxapp.conf");
	connect(_watcher, SIGNAL(fileChanged(QString)), SLOT(fileChanged(QString)));

	_timer->setInterval(2000);
	_timer->setSingleShot(true);
	connect(_timer, SIGNAL(timeout()), SLOT(update()));

	// Perform an initial update
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
	case 2:
		return tr("Sunny");
	case 3:
		return tr("Partly sunny");
	case 33:
		return tr("Clear");
	case 6:
	case 38:
		return tr("Mostly cloudy");
	case 35:
		return tr("Partly cloudy");
	case 7:
		return tr("Cloudy");
	default:
		return QString("%1").arg(_lastWxCode);
	}
}

WeatherNotification::WeatherType HarmAccuWeather::forecast()
{
	switch (_lastWxCode) {
	case 2:
	case 3:
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

void HarmAccuWeather::fileChanged(const QString &path)
{
	Q_UNUSED(path);
	qDebug() << "accuweather config file changed";
	_timer->start();
}

void HarmAccuWeather::update()
{
	QSettings* s = getAccuweatherData();

	qDebug() << "reading accuweather config file";

	QDateTime lastUpdate = s->value("LastUpdate").toDateTime();
	if (lastUpdate > _lastUpdate) {
		_lastUpdate = lastUpdate;
		_metric = s->value("useMetric").toBool();
		_lastTemp = s->value("LastTemp").toInt();
		_lastLocation = s->value("LastLocation").toString();
		_lastWxCode = s->value("LastWxCode").toInt();
		qDebug() << "updated weather info" << _lastUpdate << _lastWxCode;
		emit changed();
	}

	delete s;
}
