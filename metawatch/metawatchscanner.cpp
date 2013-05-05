#include <QtConnectivity/QBluetoothDeviceInfo>
#include <QtConnectivity/QBluetoothAddress>

#include "metawatchscanner.h"

QTM_USE_NAMESPACE
using namespace sowatch;

MetaWatchScanner::MetaWatchScanner(QObject *parent) :
	WatchScanner(parent),
	_agent(new QBluetoothServiceDiscoveryAgent(this))
{
	_agent->setUuidFilter(QBluetoothUuid::SerialPort);
	connect(_agent, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(_agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
			this, SLOT(handleDiscoveredService(QBluetoothServiceInfo)));
}

void MetaWatchScanner::start()
{
	if (_agent->isActive()) {
		_agent->stop();
	}
	_agent->start();
	qDebug() << "started metawatch bluetooth scan";
	emit started();
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
			foundInfo["next-watchlet-button"] = QString("A");
			emit watchFound(foundInfo);
		} else {
			// For now, assume Digital metawatch.
			foundInfo["driver"] = QString("metawatch-digital");
			foundInfo["next-watchlet-button"] = QString("A");
			foundInfo["idle-watchlet"] = QString("com.javispedro.sowatch.metawatch.watchface");
			foundInfo["notification-watchlet"] = QString("com.javispedro.sowatch.metawatch.notificationwatchlet");
			emit watchFound(foundInfo);
		}
	}
}
