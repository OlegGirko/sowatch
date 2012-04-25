#include "metawatchanalog.h"

using namespace sowatch;

// TODO : Figure out what to do with the buttons
// TODO : Figure out how the OLED screen protocol works, including scrolling
// TODO : Idle and Notification screens for the Analog Metawatch

MetaWatchAnalog::MetaWatchAnalog(ConfigKey* settings, QObject *parent) :
	MetaWatch(settings, parent)
{
}

int MetaWatchAnalog::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return screenWidth;
	case PdmHeight:
		return screenHeight;
	case PdmWidthMM:
		return 8;
	case PdmHeightMM:
		return 8;
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

QString MetaWatchAnalog::model() const
{
	return "metawatch-analog";
}

void MetaWatchAnalog::updateNotificationCount(Notification::Type type, int count)
{
	Q_UNUSED(type);
	Q_UNUSED(count);
	// Analog doesn't do anything with this
}

void MetaWatchAnalog::updateWeather(WeatherNotification *weather)
{
	Q_UNUSED(weather);
	// Analog doesn't do anything with this
}

void MetaWatchAnalog::displayIdleScreen()
{
	qDebug() << "displaying idle screen";
	MetaWatch::displayIdleScreen();
}

void MetaWatchAnalog::displayNotification(Notification *n)
{
	qDebug() << "display notification" << n->title() << n->body();

	// Render the notification and display it before invoking haptic feedback
	_currentMode = NotificationMode;
	// TODO

	MetaWatch::displayNotification(n);
}

void MetaWatchAnalog::displayApplication()
{
	qDebug() << "entering application mode";

	MetaWatch::displayApplication();
}

void MetaWatchAnalog::update(Mode mode, const QList<QRect> &rects)
{
	if (!_connected) return;
	Q_UNUSED(mode);
	Q_UNUSED(rects);
	// TODO
}

void MetaWatchAnalog::clear(Mode mode, bool black)
{
	if (!_connected) return;
	Q_UNUSED(mode);
	Q_UNUSED(black);
	// TODO Still need to understand this
}

void MetaWatchAnalog::handleWatchConnected()
{

}
