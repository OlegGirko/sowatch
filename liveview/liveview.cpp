#include "liveview.h"

using namespace sowatch;
QTM_USE_NAMESPACE

LiveView::LiveView(ConfigKey* settings, QObject* parent) :
	BluetoothWatch(QBluetoothAddress(settings->value("address").toString()), parent),
	_settings(settings->getSubkey(QString(), this))
{

}

QPaintEngine* LiveView::paintEngine() const
{
	return 0; // TODO
}

int LiveView::metric(PaintDeviceMetric metric) const
{
	return 0; // TODO
}

QString LiveView::model() const
{
	return "liveview";
}

QStringList LiveView::buttons() const
{
	return QStringList();
}

bool LiveView::isConnected() const
{
	return false;
}

bool LiveView::busy() const
{
	return false; // TODO
}

void LiveView::setDateTime(const QDateTime& dateTime)
{

}
void LiveView::queryDateTime()
{

}
QDateTime LiveView::dateTime() const
{
	return QDateTime::currentDateTime(); // TODO
}

void LiveView::queryBatteryLevel()
{

}
int LiveView::batteryLevel() const
{
	return 0; // TODO
}

void LiveView::queryCharging()
{

}

bool LiveView::charging() const
{
	return false; // TODO
}

void LiveView::displayIdleScreen()
{

}

void LiveView::displayNotification(Notification *notification)
{

}

void LiveView::displayApplication()
{

}

void LiveView::vibrate(int msecs)
{

}
