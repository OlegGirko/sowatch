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
	if (deviceName.contains("MetaWatch", Qt::CaseInsensitive)) {
		QVariantMap foundInfo;
		foundInfo["address"] = dev.address().toString();
		foundInfo["name"] = deviceName;
		qDebug() << "metawatch bluetooth scan found:" << deviceName;
		if (deviceName.contains("Digital", Qt::CaseInsensitive)) {
			foundInfo["driver"] = QString("metawatch-digital");
			emit watchFound(foundInfo);
		} else if (deviceName.contains("Analog", Qt::CaseInsensitive)) {
			foundInfo["driver"] = QString("metawatch-analog");
			emit watchFound(foundInfo);
		} else {
			qWarning() << "Unknown MetaWatch device found:" << deviceName;
		}
	}
}
