#include "metawatchdigital.h"

using namespace sowatch;

MetaWatchDigital::MetaWatchDigital(ConfigKey* settings, QObject *parent) :
	MetaWatch(settings, parent)
{
	QImage baseImage(screenWidth, screenHeight, QImage::Format_MonoLSB);
	baseImage.setColor(0, QColor(Qt::white).rgb());
	baseImage.setColor(1, QColor(Qt::black).rgb());
	_image[IdleMode] = baseImage;
	_image[ApplicationMode] = baseImage;
	_image[NotificationMode] = baseImage;
}

int MetaWatchDigital::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return screenWidth;
	case PdmHeight:
		return screenHeight;
	case PdmWidthMM:
		return 24;
	case PdmHeightMM:
		return 24;
	case PdmNumColors:
		return 2;
	case PdmDepth:
		return 1;
	case PdmDpiX:
	case PdmPhysicalDpiX:
		return 96;
	case PdmDpiY:
	case PdmPhysicalDpiY:
		return 96;
	}

	return -1;
}

QString MetaWatchDigital::model() const
{
	return "metawatch-digital";
}

void MetaWatchDigital::displayIdleScreen()
{
	qDebug() << "entering idle screen";
	MetaWatch::displayIdleScreen();

	// Usually, idle screen is kept updated, so we can flip it right away.
	updateLcdDisplay(IdleMode);
}

void MetaWatchDigital::displayNotification(Notification *n)
{
	qDebug() << "entering notification mode";
	MetaWatch::displayNotification(n);
}

void MetaWatchDigital::displayApplication()
{
	qDebug() << "entering application mode";
	MetaWatch::displayApplication();
}

void MetaWatchDigital::clear(Mode mode, bool black)
{
	if (!_connected) return;
	loadLcdTemplate(mode, black ? 1 : 0);
}

void MetaWatchDigital::update(Mode mode, const QList<QRect> &rects)
{
	if (!_connected) return;
	const QRect clipRect(0, 0, screenWidth, screenHeight);
	QVector<bool> lines(screenHeight, false);

	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(clipRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			lines[i] = true;
		}
	}

	updateLcdLines(mode, _image[mode], lines);
	if (mode == _currentMode) {
		updateLcdDisplay(mode);
	}
}

#if 0
QUrl MetaWatchDigital::iconForNotification(const Notification *n)
{
	switch (n->type()) {
	case Notification::CallNotification:
	case Notification::MissedCallNotification:
		return QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/metawatch/graphics/phone.png");
		break;
	case Notification::SmsNotification:
	case Notification::MmsNotification:
	case Notification::ImNotification:
		return QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/metawatch/graphics/message.png");
		break;
	case Notification::EmailNotification:
		return QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/metawatch/graphics/email.bmp");
		break;
	case Notification::CalendarNotification:
		return QUrl::fromLocalFile(SOWATCH_RESOURCES_DIR "/metawatch/graphics/timer.bmp");
		break;
	default:
		return QUrl();
	}
}

void MetaWatchDigital::renderIdleWeather()
{
	_paintMode = IdleMode;
	QFont sf("MetaWatch Small caps 8pt");
	QFont lf("MetaWatch Large 16pt");
	QPainter p(this);

	sf.setPixelSize(8);
	lf.setPixelSize(16);

	p.fillRect(0, systemAreaHeight + 6, screenWidth, systemAreaHeight - 6, Qt::white);

	if (_wForecast != WeatherNotification::UnknownWeather) {
		QImage icon = iconForWeather(_wForecast);
		QString unit = QString::fromUtf8(_wMetric ? "°C" : "°F");

		QRect bodyRect(3, systemAreaHeight + 6, 36, systemAreaHeight - 6);
		QTextOption option;
		option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		p.setFont(sf);
		p.drawText(bodyRect, _wBody, option);

		p.drawImage(36, systemAreaHeight + 6, icon);

		p.setFont(lf);
		p.drawText(63, systemAreaHeight + 23, QString("%1 %2").arg(_wTemperature).arg(unit));
	}

	_paintMode = _currentMode;
}

QImage MetaWatchDigital::iconForWeather(WeatherNotification::WeatherType w)
{
	switch (w) {
	case WeatherNotification::Sunny:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/weather_sunny.bmp"));
	case WeatherNotification::PartlyCloudy:
	case WeatherNotification::Cloudy:
	case WeatherNotification::Fog:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/weather_cloudy.bmp"));
	case WeatherNotification::Rain:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/weather_rain.bmp"));
	case WeatherNotification::Thunderstorm:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/weather_thunderstorm.bmp"));
	case WeatherNotification::Snow:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/weather_snow.bmp"));
	default:
		return QImage();
	}
}

QImage MetaWatchDigital::iconForNotification(const Notification *n)
{
	switch (n->type()) {
	case Notification::CallNotification:
	case Notification::MissedCallNotification:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/phone.bmp"));
		break;
	case Notification::SmsNotification:
	case Notification::MmsNotification:
	case Notification::ImNotification:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/message.bmp"));
		break;
	case Notification::EmailNotification:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/email.bmp"));
		break;
	case Notification::CalendarNotification:
		return QImage(QString(SOWATCH_RESOURCES_DIR "/metawatch/graphics/timer.bmp"));
		break;
	default:
		return QImage();
	}
}
#endif

void MetaWatchDigital::setupBluetoothWatch()
{
	MetaWatch::setupBluetoothWatch(); // Call generic setup

	// Grab all of the buttons that are of interest to us
	// We do not grab the F button, as it triggers the LED.
	grabButton(IdleMode, BtnA); // Required for app-switch
	grabButton(NotificationMode, BtnA);
	grabButton(NotificationMode, BtnB); // Scrolling
	grabButton(NotificationMode, BtnC);
	grabButton(ApplicationMode, BtnA);
	grabButton(ApplicationMode, BtnB);
	grabButton(ApplicationMode, BtnC);
	grabButton(ApplicationMode, BtnD);
	grabButton(ApplicationMode, BtnE);

	// Configure to show watch-rendered clock in idle screen
	// configureLcdIdleSystemArea(false); // No need to.
}
