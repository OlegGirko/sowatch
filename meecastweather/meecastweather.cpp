#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtXml/QDomComment>

#include "meecastweather.h"

using namespace sowatch;

const QLatin1String MeeCastWeather::configFilePath("/home/user/.config/com.meecast.omweather/config.xml");

MeeCastWeather::MeeCastWeather(QObject *parent) :
	WeatherNotification(parent),
	_watcher(new QFileSystemWatcher(this)),
	_timer(new QTimer(this)),
    _configFileChanged(true), _stationFileChanged(true),
	_lastUpdate(QDateTime::fromTime_t(0))
{
	_watcher->addPath(configFilePath);
	connect(_watcher, SIGNAL(fileChanged(QString)), SLOT(fileChanged(QString)));

	_timer->setInterval(10000);
	_timer->setSingleShot(true);
	connect(_timer, SIGNAL(timeout()), SLOT(handleTimeout()));

	// Perform an initial update
	parseConfigFile();
	parseStationFile();
}

Notification::Type MeeCastWeather::type() const
{
	return Notification::WeatherNotification;
}

uint MeeCastWeather::count() const
{
	return 1;
}

QDateTime MeeCastWeather::dateTime() const
{
	return _lastUpdate;
}

QString MeeCastWeather::title() const
{
	return _location;
}

QString MeeCastWeather::body() const
{
	return _lastText;
}

WeatherNotification::WeatherType MeeCastWeather::forecast()
{
	switch (_lastCode) {
	// Day versions
	case 23:
	case 24:
	case 25:
	case 32:
	case 36:
		return Sunny;
	case 30:
	case 34:
	case 44:
		return PartlyCloudy;
	case 26:
	case 28:
		return Cloudy;
	case 19:
	case 20:
	case 21:
	case 22:
		return Fog;
	case 1:
	case 2:
	case 9:
	case 10:
	case 11:
	case 12:
	case 39:
	case 40:
		return Rain;
	case 0:
	case 3:
	case 4:
	case 17:
	case 35:
	case 37:
	case 38:
		return Thunderstorm;
	case 5:
	case 6:
	case 7:
	case 8:
	case 13:
	case 14:
	case 15:
	case 16:
	case 18:
	case 41:
	case 42:
	case 43:
		return Snow;

	// Night versions
	case 31:
		return Sunny;
	case 29:
	case 33:
		return PartlyCloudy;
	case 27:
		return Cloudy;
	case 45:
		return Rain;
	case 47:
		return Thunderstorm;
	case 46:
		return Snow;

	default:
		return UnknownWeather;
	}
}

int MeeCastWeather::temperature()
{
	return _lastTemp;
}

WeatherNotification::Unit MeeCastWeather::temperatureUnits()
{
	return _tempUnit;
}

void MeeCastWeather::activate()
{
	// Launch accuweather?
}

void MeeCastWeather::dismiss()
{
	// Do nothing
}

void MeeCastWeather::fileChanged(const QString &path)
{
	qDebug() << "meecast config file changed: " << path;
	if (path == configFilePath) {
		_configFileChanged = true;
	} else if (path == _stationFilePath) {
		_stationFileChanged = true;
	}
	_timer->start();
}

void MeeCastWeather::parseConfigFile()
{
	QFile file(configFilePath);

	if (!_stationFilePath.isEmpty()) {
		_watcher->removePath(_stationFilePath);
	}
	_stationFilePath.clear();

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QDomDocument doc;
		doc.setContent(&file);

		QDomElement root = doc.documentElement();
		QDomElement e = root.firstChildElement("current_station_id");
		int stationId = e.text().toInt();

		e = root.firstChildElement("temperature_unit");
		if (e.text() == "C") {
			_tempUnit = Celsius;
		} else if (e.text() == "F") {
			_tempUnit = Fahrenheit;
		}

		QDomNodeList list = root.elementsByTagName("station");
		if (stationId >= 0 && stationId < list.size()) {
			QDomNode n = list.item(stationId);
			QDomElement e = n.toElement().firstChildElement("file_name");

			_stationFilePath = e.text();
			_watcher->addPath(_stationFilePath);
			qDebug() << "meecast found station file path: " << _stationFilePath;

			_location = n.toElement().firstChildElement("station_name").text();
		}
	}

	_configFileChanged = false;
}

void MeeCastWeather::parseStationFile()
{
	if (!_stationFilePath.isEmpty()) {
		QFile file(_stationFilePath);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QDomDocument doc;
			doc.setContent(&file);

			qDebug() << "meecast reading weather info";

			QDomElement root = doc.documentElement();
			QDomNodeList list = root.elementsByTagName("period");
			for (int index = 0; index < list.size(); index++) {
				QDomElement e = list.item(index).toElement();
				if (e.attribute("current") == "true") {
					qDebug() << "meecast current found";
					bool anythingChanged = false;
					uint periodUnix = e.attribute("start").toUInt();
					QDateTime period = QDateTime::fromTime_t(periodUnix);
					qDebug() << "period" << periodUnix << period;
					if (_lastUpdate != period) {
						anythingChanged = true;
						_lastUpdate = period;
					}

					int temp = e.firstChildElement("temperature").text().toInt();
					qDebug() << "temp" << temp;
					if (temp != _lastTemp) {
						anythingChanged = true;
						_lastTemp = temp;
					}

					int code = e.firstChildElement("icon").text().toInt();
					qDebug() << "code" << code;
					if (code != _lastCode) {
						anythingChanged = true;
						_lastCode = code;
					}

					QString text = e.firstChildElement("description").text();
					if (text != _lastText) {
						anythingChanged = true;
						_lastText = text;
					}

					if (anythingChanged) {
						qDebug() << "something changed";
						emit changed();
					}

					break;
				}
			}
		}
	}

	_stationFileChanged = false;
}

void MeeCastWeather::handleTimeout()
{
	if (_configFileChanged) {
		parseConfigFile();
	}
	if (_stationFileChanged) {
		parseStationFile();
	}
}
