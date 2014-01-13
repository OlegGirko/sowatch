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
        QRect r = rect.intersected(clipRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			lines[i] = true;
		}
	}

	updateLcdLines(mode, _image[mode], lines);
	if (mode == _currentMode) {
		updateLcdDisplay(mode);
	}
}

void MetaWatchDigital::setupBluetoothWatch()
{
	MetaWatch::setupBluetoothWatch(); // Call generic setup

	// Grab all of the buttons that are of interest to us
	// We do not grab the F button, as it triggers the LED.
	grabButton(IdleMode, BtnA); // Required for app-switch
	grabButton(NotificationMode, BtnA);
	grabButton(NotificationMode, BtnB); // Scrolling notifications
	grabButton(NotificationMode, BtnC);
	grabButton(ApplicationMode, BtnA);
	grabButton(ApplicationMode, BtnB);
	grabButton(ApplicationMode, BtnC);
	grabButton(ApplicationMode, BtnD);
	grabButton(ApplicationMode, BtnE);

	// Configure to show watch-rendered clock in idle screen
	// configureLcdIdleSystemArea(false); // No need to do this in recent firmware.
}
