#include <QtConnectivity/QBluetoothDeviceInfo>
#include <QtConnectivity/QBluetoothAddress>

#include "metawatchscanner.h"

QTM_USE_NAMESPACE
using namespace sowatch;

MetaWatchScanner::MetaWatchScanner(QObject *parent) :
	BluetoothWatchScanner(parent)
{
	setUuidFilter(QBluetoothUuid::SerialPort);
}

void MetaWatchScanner::handleDiscoveredService(const QBluetoothServiceInfo &info)
{
	const QBluetoothDeviceInfo dev = info.device();
	QString deviceName = dev.name();
	if (deviceName.startsWith("MetaWatch")) {
		QVariantMap foundInfo;
		foundInfo["address"] = dev.address().toString();
		foundInfo["name"] = deviceName;
		qDebug() << "metawatch bluetooth scan found:" << deviceName;
		if (deviceName.contains("Analog")) {
			// This is Analog metawatch.
			foundInfo["driver"] = QString("metawatch-analog");
			emit watchFound(foundInfo);
		} else {
			// For now, assume Digital metawatch.
			foundInfo["driver"] = QString("metawatch-digital");
			foundInfo["idle-watchlet"] = QString("com.javispedro.sowatch.metawatch.watchface");
			foundInfo["notification-watchlet"] = QString("com.javispedro.sowatch.metawatch.notification");
			emit watchFound(foundInfo);
		}
	}
}
