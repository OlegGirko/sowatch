#include <QtConnectivity/QBluetoothDeviceInfo>
#include <QtConnectivity/QBluetoothAddress>

#include "liveviewscanner.h"

QTM_USE_NAMESPACE
using namespace sowatch;

LiveViewScanner::LiveViewScanner(QObject *parent) :
	BluetoothWatchScanner(parent)
{
}

void LiveViewScanner::handleDiscoveredService(const QBluetoothServiceInfo &info)
{
	const QBluetoothDeviceInfo dev = info.device();
	QString deviceName = dev.name();
	if (deviceName == "LiveView") {
		QVariantMap foundInfo;
		foundInfo["driver"] = QString("liveview");
		foundInfo["address"] = dev.address().toString();
		foundInfo["name"] = deviceName;
		emit watchFound(foundInfo);
	}
}
